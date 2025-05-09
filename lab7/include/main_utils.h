#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <opencv2/imgproc.hpp>

// Usage string.
static const std::string usage = "Usage: img_stitching  \
-d <path dir with images> -a <angle for the cylindrical projection> (optional)";

// Returns the homography matrix H computed starting from the points
// corresponding the matches between two images.
cv::Mat find_homography_from_matches(
        const std::vector<cv::KeyPoint> &keypts_query,
        const std::vector<cv::KeyPoint> &keypts_train,
        const std::vector<cv::DMatch> &matches
);

// Filter the matches in 'matches' using Lowe's ratio test.
// Returns the survived mathces.
std::vector<cv::DMatch> lowe_filter(
        const std::vector<std::vector<cv::DMatch>> &matches,
        const float threshold
);

// Parse command line arguments.
void parse_command_line(int argc, char* argv[], std::string& dir_path, double &angle);

// Append all the file names in 'dir_path' in 'filenames'.
void get_all_filenames(const std::string& dir_path, std::vector<std::string>& filenames);

#endif
