#include "motor.hh"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <new>
#include <unistd.h>

#include "pos.hh"
#include "utils.hh"

#define OK "OK !\n"
#define NOK "NOK !\n"

pantilthatns::pantilthat *get_pth()
{
    pantilthatns::pantilthat *pth;

    try {
        pth = new pantilthatns::pantilthat();
    } catch (std::exception &e) {
        std::cerr << "[FATAL] could not create Pan-Tilt HAT instance class : "
                  << e.what() << "Aborting.\n"
                  << std::endl;
        exit(-1);
    }

    return pth;
}

//void clean_pth()

int move_servo(pantilthatns::pantilthat *pth, size_t servo_id, int angle)
{
    pth->set_servo(servo_id, angle);
    //printf("[PTH] move servomotor %zu to angle %d\n", servo_id, angle);
    return pth->get_servo(servo_id);
}

struct pos apply_angle(pantilthatns::pantilthat *pth, struct pos relative_pos)
{
    int angle_vertical = pth->get_servo(1);
    int angle_horizontal = pth->get_servo(2);

    float center_distance_x = fabs(relative_pos.x);
    float center_distance_y = fabs(relative_pos.y);

    int velocity_x = center_distance_x * 12;
    int velocity_y = center_distance_y * 8;

    //printf("velo x: %d, y:%d\n", velocity_x, velocity_y);

    int posx = relative_pos.x * velocity_x;
    int new_angle_horizontal =
        ((angle_horizontal + (int)(posx) + 90) ) - 90;

    int new_angle_vertical =
        ((angle_vertical - (int)(relative_pos.y * velocity_y) + 90)) - 90;

    //printf("New angle vertical:%d\n", new_angle_vertical);
    //printf("New angle horizontal:%d\n", new_angle_horizontal);

    if (new_angle_vertical > -90 && new_angle_vertical < 90)
        move_servo(pth, 1, new_angle_vertical);
    if (new_angle_horizontal > -90 && new_angle_horizontal < 90)
        move_servo(pth, 2, new_angle_horizontal);

    pos r = { pth->get_servo(1) / (float)90, pth->get_servo(2) / (float)90,
              -1 };

    return r;
}

int check_pth(pantilthatns::pantilthat *pth)
{
    unsigned char i = 0, i1 = 0, i2 = 0;
    int ii = 0, ii1 = 0, ii2 = 0;
    int err = -1;

    try {
        // May rise an std::exception if the system is unable to create an instance

        //printf("\n[INFO] checking the Pan-Tilt HAT module...\n\n");

        // Initial configuration
        if (!pth->setup()) {
            std::cerr
                << "[FATAL] Could not initialize Pan-Tilt HAT module. Aborting.\n"
                << std::endl;
            goto error;
        }
        // Reset the PTH device
        if (!pth->i2c_write_byte(REG_CONFIG, /*00001100*/ 0x0C))
            goto error;
        lsleep(50);
        // Check that Servo 1 and Servo 2 motors are disabled, that lights are
        // enabled and that Light Mode is WS2812 (Config register == 00001100 ==
        // 0x0c)
        printf("\tChecking servos and light initial status...");
        fflush(stdout);
        if (!pth->i2c_read_byte(REG_CONFIG, &i)) {
            printf(NOK);
            goto error;
        } else if (i != /*00001100*/ 0x0c) {
            printf(NOK);
            goto error;
        }
        printf(OK);
        // This configuration is known to be fool-proof
        printf("\tSetting known good config...");
        fflush(stdout);
        if ((!pth->servo_enable(1, 1)) || (!pth->servo_enable(2, 1)) ||
            (!pth->servo_pulse_min(1, 575)) ||
            (!pth->servo_pulse_max(1, 2300)) ||
            (!pth->servo_pulse_min(2, 575)) ||
            (!pth->servo_pulse_max(2, 2300))) {
            printf(NOK);
            goto error;
        }
        printf(OK);
        // Set pan then tilt position to zero
        for (i = 1; i < 3; i++) {
            printf("\tSetting servo %d to 0 degrees...", i);
            fflush(stdout);
            pth->set_servo(i, 0);
            printf(OK);
        }
        // The PTH must be able to move (-89, +89) along both axis
        printf("\tChecking ability to move...");
        fflush(stdout);
        for (ii = -89; ii < 90; ii++) {
            pth->set_servo(1, ii);
            pth->set_servo(2, ii);
            lsleep(50);
            ii1 = pth->get_servo(1);
            ii2 = pth->get_servo(2);
            if ((ii1 != ii) || (ii2 != ii)) {
                printf(NOK);
                goto error;
            }
        }
        // Set PTH head back to its zero position
        pth->set_servo(1, 0);
        pth->set_servo(2, 0);
        lsleep(1000);
        printf(OK);
        // Are we able to disable the servos ?
        printf("\tTesting servo enabling/disabling...");
        fflush(stdout);
        if ((!pth->servo_enable(1, 0)) || (!pth->servo_enable(2, 0)) ||
            (!pth->is_servo_enabled(1, &i1)) ||
            (!pth->is_servo_enabled(2, &i2)) || (i1) || (i2)) {
            printf(NOK);
            goto error;
        }
        printf(OK);

        // Getting to this point asserts no error occurred
        err = 0;
        printf("\n[SUCCESS] Pan-Tilt HAT module is fully operational.\n\n");

    error:
        // Get rid of PTH object and frees memory.
        if (pth)
            delete pth;

    } catch (std::exception &e) {
        std::cerr << "[FATAL] could not create Pan-Tilt HAT instance class : "
                  << e.what() << "Aborting.\n"
                  << std::endl;
        exit(-1);
    }

    return err;
}
