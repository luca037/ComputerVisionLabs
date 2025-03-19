#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include <opencv2/highgui.hpp>

void max_filter(const cv::Mat &in, cv::Mat &out, int sz);
void min_filter(const cv::Mat &in, cv::Mat &out, int sz);
void median_filter(const cv::Mat &in, cv::Mat &out, int sz);

#endif

