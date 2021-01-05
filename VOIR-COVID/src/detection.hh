#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "control.hh"

// Red HSV thresholds
#define RED_LOW_H 130
#define RED_HIGH_H 179

#define RED_LOW_S 130
#define RED_HIGH_S 255

#define RED_LOW_V 130
#define RED_HIGH_V 255

// Yellow HSV thresholds
#define YELLOW_LOW_H 30
#define YELLOW_HIGH_H 150

#define YELLOW_LOW_S 130
#define YELLOW_HIGH_S 255

#define YELLOW_LOW_V 200
#define YELLOW_HIGH_V 255

typedef std::vector<std::vector<cv::Point>> list_v;
typedef std::vector<cv::Point> list_p;

struct hsv_thresholds {
    unsigned H_low;
    unsigned H_high;

    unsigned S_low;
    unsigned S_high;

    unsigned V_low;
    unsigned V_high;
};

struct control detect(cv::Mat img);
cv::Point apply_thresholds(cv::Mat imgHSV, cv::Mat imgDraw,
                           struct hsv_thresholds t, cv::Scalar color);
list_p getMax(list_v l);
void remove_small_objects(cv::Mat mat);
