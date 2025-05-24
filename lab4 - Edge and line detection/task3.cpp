#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr int T2_max = 255;
int T2;

static void hough_circles(int, void *userdata) {
    // Cast input image.
    cv::Mat *img = (cv::Mat*) userdata;

    // Houg circles.
    cv::Mat output;
    cv::cvtColor(*img, output, cv::COLOR_GRAY2BGR);

    // Detect circles.
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(*img, circles, cv::HOUGH_GRADIENT, 1,
                 (float) img->rows/16, // Min distance between centers.
                 100, (T2 + 1),
                 1, 10  // Min and max radious.
    );

    for(int i = 0; i < circles.size(); i++) {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // Circle center.
        cv::circle(output, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        // Circle outline.
        int radius = c[2];
        circle(output, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
    }

    // Show result.
    cv::imshow("input", output);
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
    cv::createTrackbar("High treshold canny", "input", &T2, T2_max, hough_circles, &img);

    //Show image.
    cv::imshow("input", img);
    cv::waitKey();

    return 0;
}
