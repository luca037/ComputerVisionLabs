#include "../include/main_utils.h"

#include <iostream>
#include <opencv2/calib3d.hpp>

double mean_reproj_err(
    const std::vector<std::vector<cv::Vec3f>> &objpoints,
    const std::vector<std::vector<cv::Vec2f>> &imgpoints,
    const cv::Mat &camera_matrix,
    const cv::Mat &dist_coeff,
    const std::vector<cv::Mat> &rvecs,
    const std::vector<cv::Mat> &tvecs
) {
    double mean_error = 0;
    for (size_t i = 0; i < objpoints.size(); i++) {
        // Project the object points and store them in 'imgpoints2'.
        std::vector<cv::Vec2f> imgpoints2;
        cv::projectPoints(
                objpoints[i],
                rvecs[i],
                tvecs[i],
                camera_matrix,
                dist_coeff,
                imgpoints2
        );

        // Copute the error between the real image points and the projected ones.
        double error = cv::norm(imgpoints[i], imgpoints2) / imgpoints2.size();
        mean_error += error;
    }
    mean_error /= objpoints.size();
    return mean_error;
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
                std::cout << usage << std::endl;
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
