#include <iostream>
#include <opencv2/opencv.hpp>

#include "camera.hh"
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
    move_servo(pth, 1, -60);
    lsleep(1000);
    move_servo(pth, 2, 30);

    cv::VideoCapture cap = getVideoCapture();
    cv::Mat frame = getFrame();

    for (;;) {
        getNextFrame(cap, frame);
        //cv::imshow("picam", frame);
        pos relative_pos = getRelativeTargetPos(frame);
        pos current_pth_angle = apply_angle(pth, relative_pos);

        if (cv::waitKey(5) >= 0)
            break;
    }

    return 0;
}
