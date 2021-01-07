#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "camera.hh"

cv::VideoCapture getVideoCapture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Cannot open picam" << std::endl;
        throw std::invalid_argument( "Cannot open" );
    }

    int deviceID = 0;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);

    if (!cap.isOpened())
    {
        std::cout << "Unable to open camera" << std::endl;
        throw std::invalid_argument( "Cannot open" );
    }

    return cap;
}

cv::Mat getFrame()
{
    cv::Mat frame;

    return frame;
}

cv::Mat getNextFrame(cv::VideoCapture &cap, cv::Mat &frame)
{
    cap.read(frame);
    if (frame.empty())
        std::cout << "blank frame grabbed" << std::endl;
    return frame;
}

