#include <opencv2/highgui.hpp>

void chessbord(cv::Mat &img, int m) {
    char start_color = 0; // O is black, else is white.
    for (int i = 0; i < 300; i++) {
        // Need to invert the color?
        if ((i!= 0) && !(i % m)) { // i=0 is the edge case.
            start_color = !start_color;
        }
        // Draw black squares.
        char px_color = start_color;
        for (int j = 0; j < 300; j++) {
            // Need to invert the color?
            if ((j!= 0) && !(j % m)){ // j=0 is the edge case.
                px_color = !px_color;
            }
            // Paint the pixel if necessary.
            if (!px_color) {
                img.at<unsigned char>(i, j) = 0;
            }
        }
    }
}

int main(int argc, char** argv) {
    cv::Mat img1 = cv::Mat::zeros(256, 256, CV_8U);
    cv::Mat img2 = cv::Mat::zeros(256, 256, CV_8U);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            img1.at<unsigned char>(i, j) = j;
            img2.at<unsigned char>(i, j) = i;
        }
    }

    cv::Mat img3 = cv::Mat::zeros(300, 300, CV_8U);
    cv::Mat img4 = cv::Mat::zeros(300, 300, CV_8U);
    cv::Mat img5 = cv::Mat::zeros(300, 300, CV_8U);
    for (int i = 0; i < 300; i++) {
        for (int j = 0; j < 300; j++) {
            img3.at<unsigned char>(i, j) = std::min(j, 255);
            img4.at<unsigned char>(i, j) = std::min(i, 255);
            img5.at<unsigned char>(i, j) = std::min(j, 255);
        }
    }

    // Draw chessboard.
    chessbord(img3, 20);
    chessbord(img4, 20);
    chessbord(img5, 50);

    cv::namedWindow("win1");
    cv::namedWindow("win2");
    cv::namedWindow("win3");
    cv::namedWindow("win4");
    cv::namedWindow("win5");
    cv::imshow("win1", img1);
    cv::imshow("win2", img2);
    cv::imshow("win3", img3);
    cv::imshow("win4", img4);
    cv::imshow("win5", img5);
    cv::waitKey(0);

    return 0;
}
