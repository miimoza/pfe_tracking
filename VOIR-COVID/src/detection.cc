#include "detection.hh"

#include <iostream>
#include <vector>

using namespace cv;

struct control detect(Mat img)
{
    Mat imgHSV;
    Mat imgDraw = Mat::zeros(img.size(), CV_8UC3);

    // BGR -> HSV
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    // Seuils HSV rouge
    struct hsv_thresholds rt = { RED_LOW_H,  RED_HIGH_H, RED_LOW_S,
                                 RED_HIGH_S, RED_LOW_V,  RED_HIGH_V };

    // Seuils HSV jaune
    struct hsv_thresholds yt = { YELLOW_LOW_H,  YELLOW_HIGH_H, YELLOW_LOW_S,
                                 YELLOW_HIGH_S, YELLOW_LOW_V,  YELLOW_HIGH_V };

    // Détection des plots et centre du plot rouge le plus proche
    Point center_red = apply_thresholds(imgHSV, imgDraw, rt, Scalar(0, 0, 255));
    // Détection des plots et centre du plot jaune le plus proche
    Point center_yel =
        apply_thresholds(imgHSV, imgDraw, yt, Scalar(0, 255, 255));
    // (voir apply_thresholds)

    // Centre de l'image
    Point center_img(img.size().width / 2, img.size().height / 2);
    circle(imgDraw, center_img, 1, Scalar(255, 0, 255), 2);

    // Calcul de trajectoire
    Point base;
    Point direction;

    // -1 signifie que le plot rouge ou jaune n'est pas dans l'écran
    if (center_red.x != -1 || center_yel.x != -1) {
        // Si le plot le plus proche est plus haut que le centre de l'image,
        // on considère que le véritable plot le plus proche est hors de l'écran.
        center_red.x = (center_red.y) < img.size().height / 2 ? -1 :
                                                                center_red.x;
        center_yel.x = (center_yel.y) < img.size().height / 2 ? -1 :
                                                                center_yel.x;

        if (center_red.x != -1 && center_yel.x != -1) {
            // Trajectoire partant du milieu entre
            // les plots rouges et jaunes les plus proches
            // jusqu'au centre de l'image
            base = Point((center_red.x + center_yel.x) / 2,
                         (center_red.y + center_yel.y) / 2);
            direction = center_img;
            line(imgDraw, center_red, center_yel, Scalar(255, 0, 0), 2);

        } else {
            base = center_img;
            // Si aucun plot proche n'est présent, on avance
            if (center_red.x == -1 && center_yel.x == -1)
                direction = Point(center_img.x, img.size().height / 4);
            else
                direction = (center_red.x == -1) ?
                                Point(0, center_img.y) :
                                Point(img.size().width - 1, center_img.y);
            // Plot proche non visible, flêche jusqu'au bord de l'image
        }
    } else {
        // Si aucun plot n'est présent, on freine (fin du parcours).
        base = center_img;
        direction = center_img;
    }

    // Affichage de la trajectoire
    arrowedLine(imgDraw, base, direction, Scalar(255, 255, 0), 2);

    // Ajout des dessins sur l'image originelle
    imshow("Original", img + imgDraw);

    // Calcul des contrôles en fonction de la trajectoire
    struct control control;
    control.avant = ((base.y - direction.y) / (img.size().height / 2.0)) * 255;
    control.arriere = 0;
    control.stop = (base.y == direction.y && base.x == direction.x) ? 255 : 0;
    control.gauche =
        (base.x > direction.x) ?
            ((base.x - direction.x) / (img.size().width / 2.0)) * 255 :
            0;
    control.droite =
        (base.x < direction.x) ?
            ((direction.x - base.x) / (img.size().width / 2.0)) * 255 + 1 :
            0;

    return control;
}

Point apply_thresholds(Mat imgHSV, Mat imgDraw, struct hsv_thresholds t,
                       Scalar color)
{
    // Création d'une image binaire en applicant les seuils HSV
    Mat thresholded;
    inRange(imgHSV, Scalar(t.H_low, t.S_low, t.V_low),
            Scalar(t.H_high, t.S_high, t.V_high), thresholded);
    // Suppression des parasites
    remove_small_objects(thresholded);

    // cv::findContours permet d'obtenir une liste des plots distinct
    std::vector<std::vector<Point>> contours;
    findContours(thresholded.clone(), contours, RETR_EXTERNAL,
                 CHAIN_APPROX_SIMPLE);

    // Dessin du cercle et du centre pour chaque plot détécté
    for (auto c : contours) {
        Point2f center;
        float radius;
        float &ref_radius = radius;
        minEnclosingCircle(c, center, ref_radius);
        circle(imgDraw, center, 2, color, 2);
        circle(imgDraw, center, radius, color, 2);
    }

    // Retour du plot le plus proche (voir getMax)
    Point c(-1, -1);
    if (contours.size() > 0) {
        auto max_c = getMax(contours);
        Point2f center;
        float radius;
        float &ref_radius = radius;
        minEnclosingCircle(max_c, center, ref_radius);
        c = center;
    }
    return c;
}

// Obtention du plot le plus proche (plot le plus en bas de l'image)
list_p getMax(list_v l)
{
    float center_y_max = 0;
    list_p bottom;
    Point2f center;
    float radius;
    float &ref_radius = radius;
    for (list_p p : l) {
        minEnclosingCircle(p, center, ref_radius);
        if (center.y > center_y_max) {
            center_y_max = center.y;
            bottom = p;
        }
    }

    return bottom;
}

void remove_small_objects(cv::Mat mat)
{
    // Suppression des petits objects
    erode(mat, mat, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(mat, mat, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    // Bouchage des petits troues
    dilate(mat, mat, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(mat, mat, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}
