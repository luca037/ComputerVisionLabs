#include <iostream>
#include <opencv2/highgui.hpp>

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

    // Display the image in a window.
    cv::namedWindow("Example 1");
    cv::imshow("Example 1", img);
    cv::waitKey(0);

    return 0;
}
