#pragma once

struct control {
    unsigned char avant;
    unsigned char arriere;
    unsigned char stop;
    unsigned char gauche;
    unsigned char droite;
};

void display_control(struct control control);
