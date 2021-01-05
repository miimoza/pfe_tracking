#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "control.hh"
#include "detection.hh"

#define ESCAPE_KEY_CODE 27

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " video_file.mp4" << std::endl;
        return 0;
    }
    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        std::cout << "Cannot open the video" << std::endl;
        return 1;
    }

    std::cout << "#############################################" << std::endl;
    std::cout << "# Contrôle basé vision d’un vecteur voiture #" << std::endl;
    std::cout << "#############################################" << std::endl;

    // Boucle principale
    while (true) {
        cv::Mat img;

        // Image suivante
        bool bSuccess = cap.read(img);
        if (!bSuccess) {
            std::cout << "Cannot read a frame from video stream or end of video"
                      << std::endl
                      << "Program ended normally" << std::endl;
            return 0;
        }

        // Implémentation, voir detection.cc
        struct control control = detect(img);
        // Affichage console, control.cc
        display_control(control);

        // 33.333... ms = 30 ips
        // Échap pour quitter
        if (cv::waitKey(33) == ESCAPE_KEY_CODE) {
            std::cout << "Escape was pressed" << std::endl;
            break;
        }
    }

    // Fin
    return 0;
}
