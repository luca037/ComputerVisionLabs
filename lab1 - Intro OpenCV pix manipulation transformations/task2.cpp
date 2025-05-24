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

    // Print number of channels.
    std::cout << "Number of channles: " << img.channels() << "\n";

    // Display the image in a window.
    cv::namedWindow("Example 1");
    cv::imshow("Example 1", img);

    // Print the key pressed.
    char key_pressed = cv::waitKey(0);
    if (key_pressed == -1) {
        std::cout << "No key was pressed\n";
    } else {
        std::cout << "Key pressed: " << key_pressed << "\n";
    }

    return 0;
}
