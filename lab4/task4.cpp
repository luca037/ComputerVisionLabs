#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void segment_sky(const cv::Mat &src, cv::Mat &dst) {

    // Only pixels with blue level >= 200 survive.
    for (int i = 0; i < 160; i++) {
        for (int j = 0; j < src.cols; j++) {
            if (src.at<cv::Vec3b>(i, j)[0] >= 200) 
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b{255, 255, 255};
        }
    }

    // Pixels below row 160 are neglected.
    for (int i = 160; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dst.at<cv::Vec3b>(i, j) = cv::Vec3b{};
        }
    }

}

int main(int argc, char** argv) {
    // Try to open the img.
    char img_path[] = "./street_scene.png";
    cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);

    // Check if it was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Fist simple approach to segment the sky.
    cv::Mat simple = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    segment_sky(img, simple);

    // Convert to gray scale.
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // Apply some smoothing.
    cv::blur(img_gray, img_gray, cv::Size{3, 3});

    // Otsu optimal threshold.
    cv::Mat otsu;
    int otsu_opt = cv::threshold(img_gray, otsu, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    printf("Otsu optimal value = %d\n", otsu_opt);
    // Also with otsu we can neglect all the pixles below row 148
    for (int i = 148; i < otsu.rows; i++) {
        for (int j = 0; j < otsu.cols; j++) {
            otsu.at<uchar>(i, j) = 0;
        }
    }

    // Crate the window.
    cv::namedWindow("Gray input");
    cv::namedWindow("Simple");
    cv::namedWindow("Otsu + Blur");

    // Crate the trackbar.
    //cv::createTrackbar("High treshold canny", "input", &T2, T2_max, hough_circles, &img);

    //Show image.
    cv::imshow("Gray input", img_gray);
    cv::imshow("Simple", simple);
    cv::imshow("Otsu + Blur", otsu);
    cv::waitKey();

    return 0;
}
