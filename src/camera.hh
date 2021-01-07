#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::VideoCapture getVideoCapture();
cv::Mat getFrame();
cv::Mat getNextFrame(cv::VideoCapture &cap, cv::Mat &frame);
