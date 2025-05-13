#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/calib3d.hpp>

#include "../include/main_utils.h"


static const cv::Size patternsize(6, 5);

int main(int argc, char *argv[]) {

    // Parse command line.
    std::string dir_path{};
    parse_command_line(argc, argv, dir_path);
    if (dir_path.empty()) {
        std::cerr << usage << std::endl;
        return 1;
    }

    // Extract names of the images in the directory.
    std::vector<std::string> images_paths{};
    get_all_filenames(dir_path, images_paths);
    for (auto const &e : images_paths)
        std::cout << e << std::endl;

    // Open the images.
    std::vector<cv::Mat> images(images_paths.size());
    for (size_t i = 0; i < images.size(); i++) {
        images[i] = cv::imread(images_paths[i]);
        if (images[i].empty()) {
            std::cout << "Error - cannot open image '" 
                      << images_paths[i] << "'" << std::endl;
        }
    }

    // Prepare object points.
    std::vector<cv::Vec3f> objp;
    for (size_t i = 0; i < patternsize.width; i++) {
        for (size_t j = 0; j < patternsize.height; j++) {
            //printf("[%lu, %lu, 0]\n", j, i);
            objp.push_back({(float) j, (float) i, 0});
        }
    }
    //for (auto const &e : objp)
    //    std::cout << e << std::endl;


    // Arrays in which we store the object points (chessboard corners in 3D)
    // and the image points (chessboard corners in 2D).
    std::vector<std::vector<cv::Vec3f>> objpoints{}; // 3d point in real world space
    std::vector<std::vector<cv::Vec2f>> imgpoints{}; // 2d points in image plane.

    for (cv::Mat &img : images) {
        // Corner that we find.
        std::vector<cv::Vec2f> corners;

        bool pattern_found = cv::findChessboardCorners(img, patternsize, corners);

        if (pattern_found) {
            std::cout << "Pattenr found." << std::endl;
            
            objpoints.push_back(objp);

            cv::TermCriteria criteria(
                cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER,
                30,
                0.001
            );

            cv::Mat gray;
            cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
            //cv::cornerSubPix(gray, corners, cv::Size{5, 5}, cv::Size{-1, -1}, criteria);
            imgpoints.push_back(corners);

            // Draw corners and show them.
            //cv::drawChessboardCorners(img, patternsize, cv::Mat(corners), pattern_found);
            //cv::imshow("img", img);
            //cv::waitKey();

        } else {
            std::cout << "Patternd not found." << std::endl;
        }
    }

    // Calibration of the camera.
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(
        objpoints, 
        imgpoints,
        images[0].size(),
        cameraMatrix,
        distCoeffs,
        rvecs,
        tvecs
    );


    // Compute the mean reprojection error.
    double mean_error = 0;
    for (size_t i = 0; i < objpoints.size(); i++) {
        // Project the object points and store them in 'imgpoints2'.
        std::vector<cv::Vec2f> imgpoints2;
        cv::projectPoints(
                objpoints[i],
                rvecs[i],
                tvecs[i],
                cameraMatrix,
                distCoeffs,
                imgpoints2
        );

        // Copute the error between the real image points and the projected ones.
        double error = cv::norm(imgpoints[i], imgpoints2) / imgpoints2.size();
        mean_error += error;
    }
    mean_error /= objpoints.size();
    std::cout << "Mean reprojection error = " << mean_error << std::endl;


    // Undistortion.
    // Firstr compute the new camera matrix.
    cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(
            cameraMatrix,
            distCoeffs,
            images[0].size(),
            1,
            images[0].size()
    );

    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(
        cameraMatrix,
        distCoeffs,
        cv::Mat{},
        newCameraMatrix,
        images[0].size(),
        CV_16SC2,
        map1,
        map2
    );

    std::cout << newCameraMatrix << std::endl;

    // Remap.
    cv::Mat output;
    cv::Mat img = cv::imread("../test_image.png");
    cv::remap(img, output, map1, map2, cv::INTER_LINEAR);

    cv::imshow("ouput", output);
    //cv::imshow("src", images[0]);
    cv::waitKey(0);
    
    return 0;
}
