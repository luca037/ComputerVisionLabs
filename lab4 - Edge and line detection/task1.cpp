#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr int T1_max = 255;
constexpr int ratio = 2; // Ratio upper:lower is 2:1.
int T1;
constexpr int kernel_size = 3; // Kernel size for the Sobel filter.

static void canny_threshold(int, void *userdata) {
    // Cast input image.
    cv::Mat *img = (cv::Mat*) userdata;

    // Canny output.
    cv::Mat edges = cv::Mat::zeros(img->rows, img->cols, CV_8U);
    cv::Canny(*img, edges, T1, T1 * ratio, kernel_size, true);
    
    // Show result.
    cv::imshow("input", edges);
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

    // Convert to gray scale.
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Apply some smoothing.
    cv::blur(img, img, cv::Size{3, 3});

    // Crate the window.
    cv::namedWindow("input");

    // Crate the trackbar.
    cv::createTrackbar("T1", "input", &T1, T1_max, canny_threshold, &img);

    //Show image.
    cv::imshow("input", img);
    cv::waitKey();

    return 0;
}
