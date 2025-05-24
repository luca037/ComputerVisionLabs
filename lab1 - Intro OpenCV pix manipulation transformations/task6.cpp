#include <cmath>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void avg_filter_3x3(const cv::Mat &in, cv::Mat &out) {
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val = 0;

            // Row -1.
            if ((i - 1) >= 0 and (j - 1) >= 0) 
                px_val += in.at<unsigned char>(i - 1, j - 1);
            if ((i - 1) >= 0) 
                px_val += in.at<unsigned char>(i - 1, j);
            if ((i - 1) >= 0 and (j + 1) < in.cols) 
                px_val += in.at<unsigned char>(i - 1, j + 1);

            // Row i.
            if ((j - 1) >= 0) 
                px_val += in.at<unsigned char>(i, j - 1);
            px_val += in.at<unsigned char>(i, j);
            if ((j + 1) < in.cols) 
                px_val += in.at<unsigned char>(i, j + 1);

            // Row +1.
            if ((i + 1) >= 0 and (j - 1) >= 0) 
                px_val += in.at<unsigned char>(i + 1, j - 1);
            if ((i + 1) >= 0) 
                px_val += in.at<unsigned char>(i + 1, j);
            if ((i + 1) >= 0 and (j + 1) < in.cols) 
                px_val += in.at<unsigned char>(i + 1, j + 1);

            px_val /= 9;
            out.at<unsigned char>(i, j) = px_val;
        }
    }
}

void max_filter_3x3(const cv::Mat &in, cv::Mat &out) {
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val = 0;

            // Row -1.
            if ((i - 1) >= 0 and (j - 1) >= 0) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i - 1, j - 1));
            if ((i - 1) >= 0) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i - 1, j));
            if ((i - 1) >= 0 and (j + 1) < in.cols) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i - 1, j + 1));

            // Row i.
            if ((j - 1) >= 0) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i, j - 1));
            px_val = std::max(px_val, (int) in.at<unsigned char>(i, j));
            if ((j + 1) < in.cols) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i, j + 1));

            // Row +1.
            if ((i + 1) >= 0 and (j - 1) >= 0) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i + 1, j - 1));
            if ((i + 1) >= 0) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i + 1, j));
            if ((i + 1) >= 0 and (j + 1) < in.cols) 
                px_val = std::max(px_val, (int) in.at<unsigned char>(i + 1, j + 1));

            out.at<unsigned char>(i, j) = px_val;
        }
    }
}

void min_filter_3x3(const cv::Mat &in, cv::Mat &out) {
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val = 260;

            // Row -1.
            if ((i - 1) >= 0 and (j - 1) >= 0) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i - 1, j - 1));
            if ((i - 1) >= 0) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i - 1, j));
            if ((i - 1) >= 0 and (j + 1) < in.cols) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i - 1, j + 1));

            // Row i.
            if ((j - 1) >= 0) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i, j - 1));
            px_val = std::min(px_val, (int) in.at<unsigned char>(i, j));
            if ((j + 1) < in.cols) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i, j + 1));

            // Row +1.
            if ((i + 1) >= 0 and (j - 1) >= 0) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i + 1, j - 1));
            if ((i + 1) >= 0) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i + 1, j));
            if ((i + 1) >= 0 and (j + 1) < in.cols) 
                px_val = std::min(px_val, (int) in.at<unsigned char>(i + 1, j + 1));

            out.at<unsigned char>(i, j) = px_val;
        }
    }
}

void sobel_filter_3x3(const cv::Mat &in, cv::Mat &out) {
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            int px_val_y = 0;

            // Row -1.
            if ((i - 1) >= 0 and (j - 1) >= 0) 
                px_val_y += -1 * in.at<unsigned char>(i - 1, j - 1);
            if ((i - 1) >= 0) 
                px_val_y += -2 * in.at<unsigned char>(i - 1, j);
            if ((i - 1) >= 0 and (j + 1) < in.cols) 
                px_val_y += -1 * in.at<unsigned char>(i - 1, j + 1);

            // Row +1.
            if ((i + 1) >= 0 and (j - 1) >= 0) 
                px_val_y += in.at<unsigned char>(i + 1, j - 1);
            if ((i + 1) >= 0) 
                px_val_y += 2 * in.at<unsigned char>(i + 1, j);
            if ((i + 1) >= 0 and (j + 1) < in.cols) 
                px_val_y += in.at<unsigned char>(i + 1, j + 1);

            int px_val_x = 0;

            // Row -1.
            if ((i - 1) >= 0 and (j - 1) >= 0) 
                px_val_x += -1 * in.at<unsigned char>(i - 1, j - 1);
            if ((i - 1) >= 0 and (j + 1) < in.cols) 
                px_val_x += in.at<unsigned char>(i - 1, j + 1);

            // Row i.
            if ((j - 1) >= 0) 
                px_val_x += -2 * in.at<unsigned char>(i, j - 1);
            if ((j + 1) < in.cols) 
                px_val_x += 2 * in.at<unsigned char>(i, j + 1);

            // Row +1.
            if ((i + 1) >= 0 and (j - 1) >= 0) 
                px_val_x += -1 * in.at<unsigned char>(i + 1, j - 1);
            if ((i + 1) >= 0 and (j + 1) < in.cols) 
                px_val_x += in.at<unsigned char>(i + 1, j + 1);

            out.at<unsigned char>(i, j) = std::ceil(
                std::sqrt(
                    px_val_x * px_val_x + px_val_y * px_val_y
                )
            );
        }
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

    // Check was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Convert to gray scale.
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Avg filter.
    cv::Mat avg = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    avg_filter_3x3(img, avg);

    // Max filter.
    cv::Mat max = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    max_filter_3x3(img, max);

    // Min filter.
    cv::Mat min = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    min_filter_3x3(img, min);

    // Sobel filter.
    cv::Mat sobel = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    sobel_filter_3x3(img, sobel);

    // Display the image in a window.
    cv::namedWindow("Source img");
    cv::namedWindow("Avg filter");
    cv::namedWindow("Max filter");
    cv::namedWindow("Min filter");
    cv::namedWindow("Sobel filter");
    cv::imshow("Source img", img);
    cv::imshow("Avg filter", avg);
    cv::imshow("Max filter", max);
    cv::imshow("Min filter", min);
    cv::imshow("Sobel filter", sobel);
    cv::waitKey(0);

    return 0;
}
