#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

cv::VideoCapture getVideoCapture()
{
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Cannot open picam" << std::endl;
        return;
    }

    return cap;
}

cv::Mat getImg(cv::VideoCapture cap)
{
    cv::Mat img;
    bool bSucess = cap.read(img);
    if (!bSucess)
    {
        std::cout << "Cannot read a frame from picam" << std::endl;
        return;
    }

    return img;
}
