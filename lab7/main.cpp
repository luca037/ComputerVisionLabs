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
    std::sort(images_paths.begin(), images_paths.end());
    if (images_paths.empty()) {
        std::cerr << "No file in " << dir_path << std::endl;
        return 1;
    }

    //std::cout << "All files in " << dir_path << std::endl;
    //for (const std::string &f : images_paths)
    //    std::cout << f << "\n";

    // Open all the images.
    std::vector<cv::Mat> images(images_paths.size());
    for (int i = 0; i < images_paths.size(); i++) {
        images[i] = cv::imread(images_paths[i], cv::IMREAD_COLOR);
        if (images[i].empty()) {
            std::cerr << "Could not open " << images_paths[i] << std::endl;
            return 1;
        }
        // Equalize the histogram.
    }

    // Test with two images.
    // Project the images on a cylindrical surface.
    cv::Mat res1 = cylindricalProj(images[0], 33);
    cv::Mat res2 = cylindricalProj(images[1], 33);

    // Create the vectors for the descriptors and the keypoints.
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // Feature matching between the two above.
    cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
    // Extract features.
    detector->detectAndCompute(res1, cv::noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(res2, cv::noArray(), keypoints2, descriptors2);

    // Define the matcher.
    cv::Ptr<cv::DescriptorMatcher> matcher = 
        cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    // Define the vector of matches.
    std::vector<std::vector<cv::DMatch>> matches;
    matcher->knnMatch(descriptors1, descriptors2, matches, 2);

    // Filter the matches using Lowe's ratio test.
    std::vector<cv::DMatch> good_matches;
    const float threshold = 0.7;
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < threshold * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }

    cv::Mat img_matches;
    drawMatches(res1, keypoints1, res2, keypoints2, good_matches, img_matches, cv::Scalar::all(-1),
            cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); i++) {
        //-- Get the keypoints from the good matches
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt );
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt );
    }

    // Find homography.
    cv::Mat H = cv::findHomography(scene, obj, cv::RANSAC);
    std::cout << "Homo matrix\n" << H << std::endl;

    // Find the translation.
    int x_trans = H.at<double>(0, 2); // Translation along x.
    printf("x_trans = %d\n", x_trans);

    // Merge the two figures.
    cv::Mat outimg = cv::Mat::zeros(res1.rows, x_trans + res2.cols, CV_8U);
    // Copy res1 to the left
    cv::Rect rect1(0, 0, res1.cols, res1.rows);
    res1.copyTo(outimg(rect1));

    // Copy res2 to the right, shifted by x_trans
    cv::Rect rect2(x_trans, 0, res2.cols, res2.rows);
    res2.copyTo(outimg(rect2));

    //cv::equalizeHist(outimg, outimg);

    cv::imshow("im1", res1);
    cv::imshow("im2", res2);
    cv::imshow("Matches", img_matches);
    cv::imshow("out", outimg);
    cv::waitKey(0);

    return 0;
}
