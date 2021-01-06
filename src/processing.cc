#include "processing.hh"

static void displayMat(cv::Mat img)
{
    cv::namedWindow("DEBUG");
    cv::imshow("DEBUG", img);
    cv::waitKey(0);
    cv::waitKey(0);
    cv::waitKey(0);
    cv::waitKey(0);
    cv::waitKey(0);
    cv::destroyWindow("DEBUG");
}

struct pos getRelativeTargetPos(cv::Mat img)
{
    struct pos p = { -1, -1, -1 };
    p.x = -2;
    p.y = -2;
    p.size = -2;
    displayMat(img);
    return p;
}
