#include "./image_filter.h"
#include <algorithm>
#include <vector>

void min_filter(const cv::Mat &in, cv::Mat &out, int sz) {
    if (!(sz % 2)) {
        return;
    }

    int off = (sz - 1) / 2;
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val = 260;

            for (int z = i - off; z < i - off + sz; z++) {
                if (z < 0 or z >= in.rows) continue;
                for (int k = j - off; k < j - off + sz; k++) {
                    if (k < 0 or k >= in.cols) continue;
                    px_val = std::min(px_val, (int) in.at<unsigned char>(z, k));
                }
            }

            out.at<unsigned char>(i, j) = px_val;
        }
    }
}

void max_filter(const cv::Mat &in, cv::Mat &out, int sz) {
    if (!(sz % 2)) {
        return;
    }

    int off = (sz - 1) / 2;
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val = 0;

            for (int z = i - off; z < i - off + sz; z++) {
                if (z < 0 or z >= in.rows) continue;
                for (int k = j - off; k < j - off + sz; k++) {
                    if (k < 0 or k >= in.cols) continue;
                    px_val = std::max(px_val, (int) in.at<unsigned char>(z, k));
                }
            }

            out.at<unsigned char>(i, j) = px_val;
        }
    }
}

void median_filter(const cv::Mat &in, cv::Mat &out, int sz) {
    if (!(sz % 2)) {
        return;
    }

    int off = (sz - 1) / 2;
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {

            std::vector<int> sorted_vec{};
            for (int z = i - off; z < i - off + sz; z++) {
                if (z < 0 or z >= in.rows) continue;
                for (int k = j - off; k < j - off + sz; k++) {
                    if (k < 0 or k >= in.cols) continue;
                    sorted_vec.push_back((int) in.at<unsigned char>(z, k));
                }
            }

            std::sort(sorted_vec.begin(), sorted_vec.end());
            out.at<unsigned char>(i, j) = sorted_vec[std::floor(sorted_vec.size() / 2)];
        }
    }
}
