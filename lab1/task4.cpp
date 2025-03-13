#include <iostream>
#include <opencv2/highgui.hpp>

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

    // Print number of channels.
    std::cout << "Number of channles: " << img.channels() << "\n";
    if (img.channels() != 3) {
        return 1;
    }

    // One matrix for each channel.
    cv::Mat chanB = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    cv::Mat chanG = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    cv::Mat chanR = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);

    // Populate the 3 matrices.
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            chanB.at<cv::Vec3b>(i, j)[0] = img.at<cv::Vec3b>(i, j)[0];
            chanG.at<cv::Vec3b>(i, j)[1] = img.at<cv::Vec3b>(i, j)[1];
            chanR.at<cv::Vec3b>(i, j)[2] = img.at<cv::Vec3b>(i, j)[2];
        }
    }

    // Display the images.
    cv::namedWindow("Source img");
    cv::namedWindow("chan B");
    cv::namedWindow("chan G");
    cv::namedWindow("chan R");
    cv::imshow("Source img", img);
    cv::imshow("chan B", chanB);
    cv::imshow("chan G", chanG);
    cv::imshow("chan R", chanR);

    // Print the key pressed.
    char key_pressed = cv::waitKey(0);
    if (key_pressed == -1) {
        std::cout << "No key was pressed\n";
    } else {
        std::cout << "Key pressed: " << key_pressed << "\n";
    }

    return 0;
}
