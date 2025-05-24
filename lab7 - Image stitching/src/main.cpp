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

#include "../include/panoramic_utils.h"
#include "../include/main_utils.h"

int main(int argc, char* argv[]) {
    // Used for the cylindrical projection function.
    double angle = 33; // Default value.
    // Get the name of the directory containing the images.
    std::string dir_path{};

    // Parse command line.
    parse_command_line(argc, argv, dir_path, angle);
    if (dir_path.empty()) {
        std::cerr << usage << std::endl;
        return 1;
    }
    if (angle < 0) {
        angle = 33;
    }

    // Get the names of the files inside the directory.
    std::vector<std::string> images_paths;
    get_all_filenames(dir_path, images_paths);
    std::sort(images_paths.begin(), images_paths.end()); // Sort by name.
    if (images_paths.empty()) {
        std::cerr << "No file in '" << dir_path << "' directory" << std::endl;
        return 1;
    }

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
        if (good_matches.empty()) {
            std::cerr << "No matches survived the Lowe's ratio test..." << std::endl;
            return 1;
        }

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
