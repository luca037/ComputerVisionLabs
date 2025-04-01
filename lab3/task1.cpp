#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void call_back_print_bgr(int event, int x, int y, int flags, void *userdata) {
    cv::Mat *img = (cv::Mat*) userdata;
    if (event == cv::EVENT_LBUTTONDOWN) {
        printf("B=%d\n", (int) img->at<cv::Vec3b>(y, x)[0]);
        printf("G=%d\n", (int) img->at<cv::Vec3b>(y, x)[1]);
        printf("R=%d\n", (int) img->at<cv::Vec3b>(y, x)[2]);
    }
}

int main(int argc, char** argv) {
    // Try to open the img.
    char img_path[] = "./Robocup.jpg";
    cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR );

    // Check if it was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Convert to gray scale.
    //cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Crate the window.
    cv::namedWindow("input");

    // Set mouse call.
    cv::setMouseCallback("input", call_back_print_bgr, &img);

    //Show image.
    cv::imshow("input", img);
    cv::waitKey();

    return 0;
}
