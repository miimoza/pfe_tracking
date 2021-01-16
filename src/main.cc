#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

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

    cv::namedWindow("PFE TRACKING", CV_WINDOW_NORMAL);

    cv::VideoCapture cap = getVideoCapture();
    cap.set(cv::CAP_PROP_FPS, 15);
    cv::Mat frame = getFrame();

    cv::Mat src;
    cap >> src;
    bool isColor = (src.type() == CV_8UC3);
    cv::VideoWriter writer;
    int codec = cv::VideoWriter::fourcc('M', 'P', '4', 'V');
    std::string filename = "video.mp4";
    cv::Size sizeFrame(640, 480);
    writer.open(filename, codec, 15, sizeFrame, isColor);

    for (;;) {
        getNextFrame(cap, frame);
        writer.write(frame);

        pos relative_pos = getRelativeTargetPos(frame);
        //printf("Relative Pos (x: %f, y:%f)\n", relative_pos.x, relative_pos.y);
        pos current_pth_angle = apply_angle(pth, relative_pos);
        //printf("Current PTH Angle (x: %f, y:%f)\n", current_pth_angle.x, current_pth_angle.y);
        int key = cv::waitKey(30) & 255;
        if (key == 27)
            break;

    }
    cap.release();
    writer.release();

    return 0;
}
