#include "processing.hh"

#include <iostream>

//RED

#define LOW_H 140
#define HIGH_H 160

#define LOW_S 127
#define HIGH_S 255

#define LOW_V 192
#define HIGH_V 255

//YELLOW
/*
#define LOW_H 23
#define HIGH_H 40

#define LOW_S 41
#define HIGH_S 150

#define LOW_V 133
#define HIGH_V 255
*/

static void displayMat(cv::Mat &img)
{
    cv::Mat flipped;
    cv::flip(img, flipped, -1);
    cv::imshow("PFE TRACKING", flipped);
    //writer.write(flipped);
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
    p.size = 0;

    cv::Mat imgHSV;
    cv::resize(img, img, cv::Size(img.cols * 0.75, img.rows * 0.75), 0, 0, CV_INTER_LINEAR);
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::Mat thresholded;
    cv::inRange(imgHSV, cv::Scalar(LOW_H, LOW_S, LOW_V),
                cv::Scalar(HIGH_H, HIGH_S, HIGH_V), thresholded);

    // Suppression des parasites
    //remove_small_objects(thresholded);

    // cv::findContours permet d'obtenir une liste des plots distinct
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresholded.clone(), contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    float obj_x = 0;
    float obj_y = 0;
    size_t detected = 0;
    // Dessin du cercle et du centre pour chaque plot détécté
    for (auto c : contours) {
        cv::Point2f center;
        float radius;
        float &ref_radius = radius;
        minEnclosingCircle(c, center, ref_radius);
        obj_x += center.x / img.size().width;
        obj_y += center.y / img.size().height;
        detected++;
    }
    if (detected != 0) {
      //  std::cout << "[OCV] objet found" << std::endl;
        p.x = obj_x / detected;
        p.y = obj_y / detected;
        p.size = 8;
    }
    drawPos(img, p);
    displayMat(img);

    p.x = (p.x * 2) - 1;
    p.y = (p.y * 2) - 1;

    return p;
}
