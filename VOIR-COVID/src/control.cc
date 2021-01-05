#include "control.hh"

#include <iostream>

void display_control(struct control control)
{
    unsigned char &avant = control.avant;
    unsigned char &arriere = control.arriere;
    unsigned char &stop = control.stop;
    unsigned char &gauche = control.gauche;
    unsigned char &droite = control.droite;

    // Affiche seulement les valeurs plus grandes que zero.
    if (avant | arriere | stop | gauche | droite)
        std::cout << "==== contrôles ====" << std::endl;
    if (avant > 0)
        std::cout << "avant: " << +avant << std::endl;
    if (arriere > 0)
        std::cout << "arrière: " << +arriere << std::endl;
    if (stop > 0)
        std::cout << "stop: " << +stop << std::endl;
    if (gauche > 0)
        std::cout << "gauche: " << +gauche << std::endl;
    if (droite > 0)
        std::cout << "droite: " << +droite << std::endl;
    if (avant | arriere | stop | gauche | droite)
        std::cout << std::endl;
}
