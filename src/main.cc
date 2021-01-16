#include <iostream>
#include <opencv2/opencv.hpp>

#include "camera.hh"
#include "motor.hh"
#include "processing.hh"
#include "utils.hh"

int main(void)
{
    std::cout << "PFE TRACKING - OZAMEN" << std::endl;

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

    cv::namedWindow("picam");

    cv::VideoCapture cap = getVideoCapture();
    cap.set(cv::CAP_PROP_FPS, 15);
    cv::Mat frame = getFrame();

    for (;;) {
        getNextFrame(cap, frame);
        pos relative_pos = getRelativeTargetPos(frame);
        //printf("Relative Pos (x: %f, y:%f)\n", relative_pos.x, relative_pos.y);
        pos current_pth_angle = apply_angle(pth, relative_pos);
        //printf("Current PTH Angle (x: %f, y:%f)\n", current_pth_angle.x, current_pth_angle.y);

        if (cv::waitKey(5) >= 0)
            break;
    }

    return 0;
}
