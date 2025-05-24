#include "../include/main_utils.h"

#include <opencv2/calib3d.hpp>
#include <stdexcept>

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
void parse_command_line(int argc, char* argv[], std::string& dir_path, double &angle) {
    int opt;
    while ((opt = getopt(argc, argv, "d:a:")) != -1) {
        switch (opt) {
            case 'd':
                dir_path = optarg;
                break;
            case 'a':
                try {
                angle = std::stod(optarg);
                } catch (const std::invalid_argument &ia) {
                    std::cerr << "Invalid angle parameter: " << ia.what() << std::endl;
                    std::cerr << "Fixed to defalut value" << std::endl;
                }
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
