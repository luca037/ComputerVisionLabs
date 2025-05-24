#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <opencv2/imgproc.hpp>

// Usage string.
static const std::string usage = "Usage: camera_calib -d <path dir with images>";

// Return th emean reprojection error.
double mean_reproj_err(
    const std::vector<std::vector<cv::Vec3f>> &objpoints,
    const std::vector<std::vector<cv::Vec2f>> &imgpoints,
    const cv::Mat &camera_matrix,
    const cv::Mat &dist_coeff,
    const std::vector<cv::Mat> &rvecs,
    const std::vector<cv::Mat> &tvecs
);

// Parse command line arguments.
void parse_command_line(int argc, char* argv[], std::string& dir_path);

// Append all the file names in 'dir_path' in 'filenames'.
void get_all_filenames(const std::string& dir_path, std::vector<std::string>& filenames);

#endif
