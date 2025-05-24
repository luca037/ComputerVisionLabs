#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr int T1_max = 255;
constexpr int ratio = 2; // Ratio upper:lower is 2:1.
int T1;
constexpr int kernel_size = 3; // Kernel size for the Sobel filter.

static void canny_hugh(int, void *userdata) {
    // Cast input image.
    cv::Mat *img = (cv::Mat*) userdata;

    // Canny output.
    cv::Mat edges = cv::Mat::zeros(img->rows, img->cols, CV_8U);
    cv::Canny(*img, edges, T1, T1 * ratio, kernel_size, true);

    // Init result image.
    cv::Mat res{img->rows, img->cols};
    cv::cvtColor(edges, res, cv::COLOR_GRAY2BGR);

    // Detect lines.
    std::vector<cv::Vec2f> lines; // Will hold the results of the detection.
    cv::HoughLines(edges, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
 
    // Draw the lines
    for(size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        float deg = 180 * theta / CV_PI;
        if (deg == 45 or deg == 132) {
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line(res, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
        }
    }
    
    // Show result.
    cv::imshow("input", res);
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

    // Crate the window.
    cv::namedWindow("input");

    // Crate the trackbar.
    cv::createTrackbar("T1", "input", &T1, T1_max, canny_hugh, &img);

    //Show image.
    cv::imshow("input", img);
    cv::waitKey();

    return 0;
}
