#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void call_back_print_bgr(int event, int x, int y, int flags, void *userdata) {
    // Threshold.
    static constexpr int T = 70;

    // Input image.
    cv::Mat *img = (cv::Mat*) userdata;

    // The mask.
    cv::Mat mask = cv::Mat::zeros(img->rows, img->cols, CV_8U);

    // New image.
    cv::Mat new_image{img->rows, img->cols, CV_8UC3, cv::Scalar{92, 37, 201}};

    int mean_h{}, mean_s{}, mean_v{};
    if (event == cv::EVENT_LBUTTONDOWN) {
        printf("H=%d\n", (int) img->at<cv::Vec3b>(y, x)[0]);
        printf("S=%d\n", (int) img->at<cv::Vec3b>(y, x)[1]);
        printf("V=%d\n", (int) img->at<cv::Vec3b>(y, x)[2]);

        // Copute the mean for each channel (9x9 neighborhood).
        for (int i = y - 4; i < (y - 4) + 9; i++) {
            if (i < 0 or i >= img->rows) continue;
            for (int j = x - 4; j < (x - 4) + 9; j++) {
                if (j < 0 or j >= img->cols) continue;
                mean_h += (int) img->at<cv::Vec3b>(i, j)[0];
                mean_s += (int) img->at<cv::Vec3b>(i, j)[1];
                mean_v += (int) img->at<cv::Vec3b>(i, j)[2];
            }
        }
        mean_h /= 81; mean_s /= 81; mean_v /= 81;

        // Print the mean for each channel.
        printf("mean hue = %d\n", mean_h);
        printf("mean saturation = %d\n", mean_s);
        printf("mean value = %d\n", mean_v);

        // Fill the mask.
        for (int i = 0; i < img->rows; i++) {
            for (int j =0; j < img->cols; j++) {
                cv::Vec3b &px = img->at<cv::Vec3b>(i, j);
                if (std::abs((int) px[0] - mean_h) <= T and 
                    std::abs((int) px[1] - mean_s) <= T and
                    std::abs((int) px[2] - mean_v) <= T
                ) {
                    mask.at<uchar>(i, j) = 255;
                } else {
                    new_image.at<cv::Vec3b>(i, j) = img->at<cv::Vec3b>(i, j);
                }
            }
        }

        // Print the mask.
        cv::namedWindow("mask");
        cv::namedWindow("new image");
        cv::imshow("mask", mask);
        cv::imshow("new image", new_image);
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
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);

    // Crate the window.
    cv::namedWindow("input");

    // Set mouse call.
    cv::setMouseCallback("input", call_back_print_bgr, &img);

    //Show image.
    cv::imshow("input", img);
    cv::waitKey();

    return 0;
}
