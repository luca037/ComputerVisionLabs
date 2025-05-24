#include <iostream>
#include <opencv2/highgui.hpp>

int rm_b(cv::Mat& img) {
    if (3 != img.channels()) {
        return 1;
    }

    // Remove blue channel.
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            img.at<cv::Vec3b>(i, j)[1] = 0;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Error - Usge" << argv[0]  << " " << "<imgpath>" << "\n";
        return 1;
    }
    
    // Try to open the img.
    char* img_path = argv[1];
    cv::Mat img = cv::imread(img_path);

    // Check was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Print number of channels.
    std::cout << "Number of channles: " << img.channels() << "\n";

    // Display the image in a window.
    cv::namedWindow("win1");
    cv::imshow("win1", img);

    //// Print the key pressed.
    //char key_pressed = cv::waitKey(0);
    //if (key_pressed == -1) {
    //    std::cout << "No key was pressed\n";
    //} else {
    //    std::cout << "Key pressed: " << key_pressed << "\n";
    //}

    // Remove blue channel.
    if (rm_b(img)) {
        std::cout << "Blue channel was not removed\n";
        return 0;
    }

    // Print the image.
    cv::namedWindow("win2");
    cv::imshow("win2", img);
    cv::waitKey(0);

    return 0;
}
