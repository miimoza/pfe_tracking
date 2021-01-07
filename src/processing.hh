#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "pos.hh"

// return the detected position with x, y = ]0; 1[
struct pos getRelativeTargetPos(cv::Mat img);
