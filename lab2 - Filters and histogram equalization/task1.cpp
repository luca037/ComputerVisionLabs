#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main(int argc, char** argv) {
    //if (argc < 2) {
    //    std::cout << "Error - Usge" << argv[0]  << " " << "<imgpath>" << "\n";
    //    return 1;
    //}
    
    // Try to open the img.
    char img_path[] = "./Garden.jpg";
    cv::Mat img = cv::imread(img_path);

    // Check if it was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Convert to gray scale.
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Save the image.
    if (cv::imwrite("./Garden_grayscale.jpg", img)) {
        std::cout << "Image saved.\n";
    } else {
        std::cout << "Error - image not saved\n";
        return 1;
    }
        
    return 0;
}
