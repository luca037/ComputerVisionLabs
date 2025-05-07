#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include "./PanoramicUtils/header/panoramic_utils.h"
#include "opencv2/highgui.hpp"


// Used for the cylindrical projection function.
static constexpr double angle = 33;


// Returns the homography matrix H computed starting from the points
// corresponding the matches between two images.
cv::Mat find_homography_from_matches(
        const std::vector<cv::KeyPoint> &keypts_query,
        const std::vector<cv::KeyPoint> &keypts_train,
        const std::vector<cv::DMatch> &matches
) {
    std::vector<cv::Point2f> pt1, pt2;
    for (size_t i = 0; i < matches.size(); i++) {
        // Get the keypoints from the matches.
        pt1.push_back(keypts_query[matches[i].queryIdx].pt);
        pt2.push_back(keypts_train[matches[i].trainIdx].pt);
    }

    // Return the homography.
    return cv::findHomography(pt2, pt1, cv::RANSAC);
}

// Filter the matches in 'matches' using Lowe's ratio test.
// Returns the survived mathces.
std::vector<cv::DMatch> lowe_filter(
        const std::vector<std::vector<cv::DMatch>> &matches,
        const float threshold
) {
    std::vector<cv::DMatch> good_matches;
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < threshold * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }
    return good_matches;
}

// Parse command line arguments.
void parse_command_line(int argc, char* argv[], std::string& dir_path) {
    int opt;
    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
            case 'd':
                dir_path = optarg;
                break;
            case '?':
                std::cerr << "Usage: " << argv[0] << " -d <path>" << std::endl;
                break;
        }
    }
}

// Append all the file names in 'dir_path' in 'filenames'.
void get_all_filenames(const std::string& dir_path, std::vector<std::string>& filenames) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dir_path.c_str())) != NULL) {
        // process all the files insider the directory
        while ((ent = readdir (dir)) != NULL) {
            std::string file_name = ent->d_name;
            // Don't consider the current directory '.' and the parent ".."
            if (file_name == "." || file_name == "..") {
                continue;
            }
            if (*(dir_path.end() - 1) == '/') {
                filenames.push_back(dir_path + file_name);
            } else {
                filenames.push_back(dir_path + "/" + file_name);
            }
        }
        closedir(dir); // Close the directory.
    }
}

int main(int argc, char* argv[]) {
    // Get the name of the directory containing the images.
    std::string dir_path{};
    parse_command_line(argc, argv, dir_path);
    //std::cout << "Dir path: " << dir_path << std::endl;

    // Get the names of the files inside the directory.
    std::vector<std::string> images_paths;
    get_all_filenames(dir_path, images_paths);
    std::sort(images_paths.begin(), images_paths.end()); // Sort by name.
    if (images_paths.empty()) {
        std::cerr << "No file in " << dir_path << std::endl;
        return 1;
    }

    //std::cout << "All files in " << dir_path << std::endl;
    //for (const std::string &f : images_paths)
    //    std::cout << f << "\n";

    // Open all the images and apply cylindrical Projection to each.
    std::vector<cv::Mat> images(images_paths.size());
    for (int i = 0; i < images_paths.size(); i++) {
        images[i] = cv::imread(images_paths[i], cv::IMREAD_COLOR);
        if (images[i].empty()) {
            std::cerr << "Could not open " << images_paths[i] << std::endl;
            return 1;
        }
        // Apply the cylindrical Projection to each of them.
        images[i] = cylindricalProj(images[i], angle);
    }

    // Define the SIFT detector.
    cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
    // Define the matcher.
    cv::Ptr<cv::DescriptorMatcher> matcher = 
        cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    // Store all the keypoints vector in a vector.
    std::vector<std::vector<cv::KeyPoint>> images_keypoints(images.size());
    // Store all the descriptors in a vector.
    std::vector<cv::Mat> images_descriptors(images.size());

    // Extract the keypoints and compute the descriptors for each image.
    for (size_t i = 0; i < images.size(); i++) {
        detector->detectAndCompute(
                images[i], 
                cv::noArray(), 
                images_keypoints[i],
                images_descriptors[i]
        );
    }

    // Vector in which we store the translations.
    // transtions[i] contains the x displacement between the frame of
    // images[i-1] and images[i].
    std::vector<int> translations; 

    // Compute the x translations between each images.
    for (size_t i = 1; i < images.size(); i++) {
        // Compute the matches between the two images.
        std::vector<std::vector<cv::DMatch>> matches;
        matcher->knnMatch(
                images_descriptors[i-1], 
                images_descriptors[i], 
                matches,
                2
        );

        // Filter the matches using Lowe's ratio.
        std::vector<cv::DMatch> good_matches = lowe_filter(matches, 0.7);

        // Find homography matrix.
        cv::Mat H = find_homography_from_matches(
                images_keypoints[i-1],
                images_keypoints[i],
                good_matches
        );

        // The x displacemen is given by the element h_{33} of matrix H.
        int x_trans = (int) H.at<double>(0, 2);
        //std::cout << "x_trans = " << x_trans << std::endl;
        translations.push_back(x_trans);
    }

    
    // Compute the total number of columns of the output image.
    // Which is given by the sum of all the transitions + the num of cols
    // of the last image in the vector 'images'.
    int tot_cols = 0;
    for (std::vector<int>::iterator it = translations.begin(); it != translations.end(); it++) {
        tot_cols += *it;
    }
    tot_cols += images.back().cols;

    // Create the output image.
    cv::Mat outimg = cv::Mat::zeros(images[0].rows, tot_cols, CV_8U);
    int starting_x = 0; // Starting column of 'outimag' from which we start
                        // copying 'image[i]'.
    for (size_t i = 0; i <= translations.size(); i++) {
        // Region of 'outimg' in which we copy 'image[i]'.
        cv::Rect rect(starting_x, 0, images[i].cols, images[i].rows);

        // Equalize histogram before copying to outimg (better result).
        cv::equalizeHist(images[i], images[i]);

        // Copy 'images[i]' to the defined region of 'outimg'.
        images[i].copyTo(outimg(rect));
        
        // Update the starting column.
        if (i < translations.size()) {
            starting_x += translations[i];
        }
    }

    cv::imshow("out", outimg);
    cv::waitKey(0);

    return 0;
}
