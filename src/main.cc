#include <iostream>
#include <opencv2/opencv.hpp>

#include "processing.hh"

int main(void)
{
    std::cout << "Hello mec" << std::endl;

    cv::Mat image = cv::imread("./images/test3.png");
    getRelativeTargetPos(image);
    return 0;
}
