
#include <errno.h>
#include <exception>
#include <iostream>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "pantilthat.h"

#define OK "OK !\n"
#define NOK "NOK !\n"
#define ONESECOND 1000
#define FIFTYMILLIS 50

using namespace std;
using namespace pantilthatns;

// Suspend this thread for ms milliseconds.
int lsleep(long ms)
{
    int result = 0;
    struct timespec ts_sleep, ts_remaining;
    ts_remaining.tv_sec = (time_t)(ms / 1000);
    ts_remaining.tv_nsec = (long)(ms % 1000) * 1000000;
    do {
        ts_sleep = ts_remaining;
        result = nanosleep(&ts_sleep, &ts_remaining);
    } while ((errno == EINTR) && (result == -1));
    if (result == -1) {
        cerr << __FILE__ << ":" << __LINE__ << ":" << __func__
             << " nanosleep() failed" << endl;
    }
    return result;
}

// Get Pan Tilt Hat Object
pantilthat *get_pth()
{
    pantilthat *pth;

    try {
        pth = new pantilthat();
    } catch (exception &e) {
        cerr << "[FATAL] could not create Pan-Tilt HAT instance class : "
             << e.what() << "Aborting.\n"
             << endl;
        exit(-1);
    }

    return pth;
}

// Check Pan Tilt Hat
int check_pth(pantilthat *pth)
{
    unsigned char i = 0, i1 = 0, i2 = 0;
    int ii = 0, ii1 = 0, ii2 = 0;
    int err = -1;

    try {
        // May rise an exception if the system is unable to create an instance

        printf("\n[INFO] checking the Pan-Tilt HAT module...\n\n");

        // Initial configuration
        if (!pth->setup()) {
            cerr
                << "[FATAL] Could not initialize Pan-Tilt HAT module. Aborting.\n"
                << endl;
            goto error;
        }
        // Reset the PTH device
        if (!pth->i2c_write_byte(REG_CONFIG, /*00001100*/ 0x0C))
            goto error;
        lsleep(FIFTYMILLIS);
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
        lsleep(ONESECOND);
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

    } catch (exception &e) {
        cerr << "[FATAL] could not create Pan-Tilt HAT instance class : "
             << e.what() << "Aborting.\n"
             << endl;
        exit(-1);
    }

    return err;
}

int move_servo(pantilthat *pth, size_t servo_id, float angle)
{
    pth->set_servo(servo_id, angle);
    printf("[PTH] move servomotor %ld to angle %f\n", servo_id, angle);
    return pth->get_servo(servo_id);
}

int main()
{
    pantilthat *pth = get_pth();
    move_servo(pth, 1, -60);
    lsleep(ONESECOND);
    move_servo(pth, 2, 30);

    return 0;
}
