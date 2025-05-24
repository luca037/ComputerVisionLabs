#include "opencv2/core/matx.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main(int argc, char** argv) {
    // Try to open the img.
    char img_path[] = "./street_scene.png";
    cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);

    // Check if it was opened.
    if (img.data == NULL) {
        std::cout << "Error - '" << img_path << "' cannot be opened\n";
        return 1;
    }

    // Convert to gray scale (Canny output is better starting from this image).
    cv::Mat img_gray{img.rows, img.cols};
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // Canny output.
    cv::Mat edges = cv::Mat::zeros(img.rows, img.cols, CV_8U);
    cv::Canny(img_gray, edges, 140, 280, 3, true);

    // Image with the red triangle.
    cv::Mat red_triangle = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);

    // Detect lines with Hough transform.
    std::vector<cv::Vec2f> lines; // Will hold the results of the detection.
    cv::HoughLines(edges, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
 
    // Find the two requested lines.
    for(int i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        float deg = 180 * theta / CV_PI; // Convert theta in degrees.

        // Detect the two lines and draw them.
        if (deg == 45 or deg == 132) {
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cv::line(red_triangle, pt1, pt2, cv::Scalar(0,0,255), 1);
        }
    }

    // Remove the inverted red triangle 
    //      => color in black all the pixels from row 0 to row 180.
    for (int i = 0; i < 180; i++) {
        for (int j = 0; j < red_triangle.cols; j++) {
            red_triangle.at<cv::Vec3b>(i, j) = cv::Vec3b{};
        }
    }

    // Fill the triangle with red (start from row 180).
    for (int i = 180; i < red_triangle.rows; i++) {

        // First count the number of red pixels in row i;
        int red_cnt = 0;
        for (int j = 0; j < red_triangle.cols; j++) {
            if (red_triangle.at<cv::Vec3b>(i, j)[2]  != 0) {
                red_cnt += 1;
            }
        }

        // Now start coloring the row.
        //      => start coloring when you find the first red pixel
        //      => stop when you have encountered 'red_cnt' red pixels
        int cnt = 0; // Red pixels encountered.
        for (int j = 0; j < red_triangle.cols; j++) {
            if (red_triangle.at<cv::Vec3b>(i, j)[2]  == 255) { 
                cnt += 1;
            }
            else if (cnt > 0 
                    and cnt < red_cnt 
                    and red_triangle.at<cv::Vec3b>(i, j)[2] == 0) {
                red_triangle.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    }

    // Create the final result image.
    cv::Mat output = red_triangle.clone();
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (output.at<cv::Vec3b>(i, j)[2] == 0) {
                output.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, j);
            } 
        }
    }

    // Crate the window.
    cv::namedWindow("Red Triangle");
    cv::namedWindow("Output");

    //Show image.
    cv::imshow("Red Triangle", red_triangle);
    cv::imshow("Output", output);
    cv::waitKey();

    return 0;
}
