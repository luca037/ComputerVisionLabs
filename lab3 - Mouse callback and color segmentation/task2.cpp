#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void call_back_print_bgr(int event, int x, int y, int flags, void *userdata) {
    cv::Mat *img = (cv::Mat*) userdata;
    int mean_b{}, mean_g{}, mean_r{};
    if (event == cv::EVENT_LBUTTONDOWN) {
        printf("B=%d\n", (int) img->at<cv::Vec3b>(y, x)[0]);
        printf("G=%d\n", (int) img->at<cv::Vec3b>(y, x)[1]);
        printf("R=%d\n", (int) img->at<cv::Vec3b>(y, x)[2]);

        // Copute the mean for each channel (9x9 neighborhood).
        for (int i = y - 4; i < (y - 4) + 9; i++) {
            if (i < 0 or i >= img->rows) continue;
            for (int j = x - 4; j < (x - 4) + 9; j++) {
                if (j < 0 or j >= img->cols) continue;
                mean_b += (int) img->at<cv::Vec3b>(i, j)[0];
                mean_g += (int) img->at<cv::Vec3b>(i, j)[1];
                mean_r += (int) img->at<cv::Vec3b>(i, j)[2];
            }
        }
        mean_b /= 81; mean_g /= 81; mean_r /= 81;

        // Print the mean for each channel.
        printf("mean blue = %d\n", mean_b);
        printf("mean green = %d\n", mean_g);
        printf("mean red = %d\n", mean_r);
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
