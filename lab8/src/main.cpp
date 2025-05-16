#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "../include/main_utils.h"

// Define the pattern size.
static const cv::Size pattern_size(6, 5);

int main(int argc, char *argv[]) {

    // Parse command line: get dataset directory.
    std::string dir_path{};
    parse_command_line(argc, argv, dir_path);
    if (dir_path.empty()) {
        std::cerr << usage << std::endl;
        return 1;
    }

    // Extract names of the images in the directory.
    std::vector<std::string> images_paths{};
    get_all_filenames(dir_path, images_paths);
    if (images_paths.empty()) {
        std::cerr << usage << std::endl;
        return 1;
    }

    // Open the images and store them in a vector.
    std::vector<cv::Mat> images(images_paths.size());
    for (size_t i = 0; i < images.size(); i++) {
        images[i] = cv::imread(images_paths[i]);
        if (images[i].empty()) {
            std::cerr << "Error - cannot open image '" 
                      << images_paths[i] << "'" << std::endl;
            return 1;
        }
    }

    // Prepare object points.
    std::vector<cv::Vec3f> objp;
    for (size_t i = 0; i < pattern_size.height; i++) {
        for (size_t j = 0; j < pattern_size.width; j++) {
            objp.push_back({(float) j, (float) i, 0});
        }
    }

    // Define an array in which we store the object points (the ones that
    // lies in the 3D object plane).
    std::vector<std::vector<cv::Vec3f>> objpoints{};
    // Define an array in which we store the image points (the ones that
    // lies in the 2D image plane).
    std::vector<std::vector<cv::Vec2f>> imgpoints{};

    for (cv::Mat &img : images) {
        // Find the corner points (image plane).
        std::vector<cv::Vec2f> corners;
        bool pattern_found = cv::findChessboardCorners(
            img,
            pattern_size, // Defined as global variable.
            corners
        );

        // If we have found the defined pattern.
        if (pattern_found) {
            // Add the 3D object points.
            objpoints.push_back(objp);
            // Add the 2D image points.
            imgpoints.push_back(corners);

            // Draw corners and show them (Used for debugging).
            //cv::drawChessboardCorners(img, patternsize, cv::Mat(corners), pattern_found);
            //cv::imshow("img", img);
            //cv::waitKey();

        } else {
            std::cout << "Patternd not found." << std::endl;
        }
    }

    // Calibrate the camera.
    cv::Mat camera_matrix;      // The camera matrix.
    cv::Mat dist_coeff;         // Distortion coefficients.
    std::vector<cv::Mat> rvecs; // Rotation vectors for each pattern view.
    std::vector<cv::Mat> tvecs; // Translation vector for each parttern view.
    cv::calibrateCamera(
        objpoints, 
        imgpoints,
        images[0].size(), // We assume all images have same size.
        camera_matrix,
        dist_coeff,
        rvecs,
        tvecs
    );


    // Compute mean reprojection error.
    double mean_error = mean_reproj_err(
        objpoints,
        imgpoints,
        camera_matrix,
        dist_coeff,
        rvecs,
        tvecs
    );
    std::cout << "Mean reprojection error = " << mean_error << std::endl;


    // Now use the paramter to undistort the images.
    // Refine the camera matrix.
    cv::Mat new_camera_matrix = cv::getOptimalNewCameraMatrix(
        camera_matrix,
        dist_coeff,
        images[0].size(), // We assume all images have same size.
        0,
        images[0].size()
    );

    // Compute the maps to undistort the images.
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(
        camera_matrix,
        dist_coeff,
        cv::Mat{}, // => Use identity as rotation matrix R.
        new_camera_matrix,
        images[0].size(), // We assume all images have same size.
        CV_32FC1, 
        map1,
        map2
    );

    // Undistort images
    cv::namedWindow("output", cv::WINDOW_NORMAL);
    for (const cv::Mat &img : images) {
        cv::Mat undistorted_img;
        cv::remap(img, undistorted_img, map1, map2, cv::INTER_LINEAR);

        // Concatenate the distorted 'img' and the undistorted image.
        cv::Mat concat;
        cv::hconcat(img, undistorted_img, concat);

        // Show image.
        cv::imshow("output", concat);
        cv::waitKey(0);
    }
    
    return 0;
}
