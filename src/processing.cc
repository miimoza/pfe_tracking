#include "processing.hh"

#include <iostream>

#define LOW_H 140
#define HIGH_H 160

#define LOW_S 127
#define HIGH_S 255

#define LOW_V 192
#define HIGH_V 255

static void displayMat(cv::Mat &img)
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

static void drawPos(cv::Mat &img, struct pos p)
{
    cv::Point point(p.x * img.size().width, p.y * img.size().height);
    cv::circle(img, point, p.size, cv::Scalar(0, 255, 0), -1);
}

static void remove_small_objects(cv::Mat mat)
{
    // Suppression des petits objects
    cv::erode(mat, mat,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(mat, mat,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    // Bouchage des petits troues
    cv::dilate(mat, mat,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(mat, mat,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
}

struct pos getRelativeTargetPos(cv::Mat img)
{
    struct pos p = { -1, -1, -1 };
    p.x = 0.5;
    p.y = 0.5;
    p.size = 50;

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::Mat thresholded;
    cv::inRange(imgHSV, cv::Scalar(LOW_H, LOW_S, LOW_V),
                cv::Scalar(HIGH_H, HIGH_S, HIGH_V), thresholded);

    // Suppression des parasites
    remove_small_objects(thresholded);

    // cv::findContours permet d'obtenir une liste des plots distinct
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresholded.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    float obj_x = 0;
    float obj_y = 0;
    size_t detected = 0;
    // Dessin du cercle et du centre pour chaque plot détécté
    for (auto c : contours) {
        std::cout << "[OCV] objet found" << std::endl;
        cv::Point2f center;
        float radius;
        float &ref_radius = radius;
        minEnclosingCircle(c, center, ref_radius);
        obj_x += center.x / img.size().width;
        obj_y += center.y / img.size().height;
        detected++;
    }
    p.x = obj_x / detected;
    p.y = obj_y / detected;
    drawPos(img, p);
    displayMat(img);
    return p;
}
