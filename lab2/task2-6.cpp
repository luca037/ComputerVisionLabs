#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "./image_filter.h"

void plot_hist(const cv::Mat &src, cv::Mat &plot, int histsz) {
    // Histogram info.
    int nch = 0; // Num of channels.
    cv::Mat hist{}; // The histogram.
    float range[] = {0., 256.}; 
    const float *ranges[] = {range};
    cv::calcHist(&src, 1, &nch, cv::Mat(), hist, 1, &histsz, ranges);

    // Normalize the histogram to fit within the image height
    cv::normalize(hist, hist, 0, plot.rows, cv::NORM_MINMAX);

    // Draw the histogram.
    int bin_w = cvRound((double) plot.cols/ histsz); // Set bin width.
    for (int i = 0; i < histsz; i++) {
        cv::rectangle(plot,
                  cv::Point(bin_w * i, plot.rows),
                  cv::Point(bin_w * (i + 1), plot.rows - cvRound(hist.at<float>(i))),
                  cv::Scalar(0), cv::FILLED);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Error - Usge" << argv[0]  << " " << "<imgpath>" << "\n";
        return 1;
    }
    
    // Try to open the img.
    char *img_path = argv[1];
    cv::Mat img = cv::imread(img_path);

    // Check if it was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Convert to gray scale.
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Apply filters.
    cv::Mat max = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    cv::Mat min = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    cv::Mat median = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    cv::Mat gaussian = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    max_filter(img, max, 3);
    min_filter(img, min, 3);
    median_filter(img, median, 5);
    cv::GaussianBlur(img, gaussian, cv::Size{7, 7}, 2);

    // Compute histogram.
    cv::Mat hist{512, 700, CV_8U, cv::Scalar{255}};
    plot_hist(img, hist, 256);

    // Histogram equalization.
    cv::Mat img_eq = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    cv::equalizeHist(img, img_eq);

    cv::Mat hist_eq{512, 700, CV_8U, cv::Scalar{255}};
    plot_hist(img_eq, hist_eq, 256);

    // Show the image.
    cv::namedWindow("Source img");
    cv::namedWindow("Max filter");
    cv::namedWindow("Min filter");
    cv::namedWindow("Median filter");
    cv::namedWindow("Gaussian filter");
    cv::namedWindow("Histogram plot");
    cv::namedWindow("Equalized histogram plot");
    cv::namedWindow("Source img equalized");
    cv::imshow("Source img", img);
    cv::imshow("Max filter", max);
    cv::imshow("Min filter", min);
    cv::imshow("Median filter", median);
    cv::imshow("Gaussian filter", gaussian);
    cv::imshow("Histogram plot", hist);
    cv::imshow("Equalized histogram plot", hist_eq);
    cv::imshow("Source img equalized", img_eq);
    cv::waitKey();
        
    return 0;
}

// To remover the salt-pepper noise the best filter is the median with
// size 5x5. The problem is that we loose many information about
// the edges.

// Max filter is the best option for removing the electric cables.
// Kernel of size 5x5 make them disappear.
