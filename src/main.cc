#include <iostream>
#include <opencv2/opencv.hpp>

#include "motor.hh"
#include "processing.hh"
#include "utils.hh"

int main(void)
{
    std::cout << "Hello mec" << std::endl;

    /*
    cv::Mat image = cv::imread("./images/test3.png");
    getRelativeTargetPos(image);
    image = cv::imread("./images/test2.png");
    getRelativeTargetPos(image);
    image = cv::imread("./images/test.png");
    getRelativeTargetPos(image);*/

    pantilthatns::pantilthat *pth = get_pth();
    move_servo(pth, 1, 60);
    lsleep(1000);
    move_servo(pth, 2, -30);

    return 0;
}
