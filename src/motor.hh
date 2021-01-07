#pragma once

#include "libpth/pantilthat.h"

// Get Pan Tilt Hat Object
pantilthatns::pantilthat *get_pth();

// Check Pan Tilt Hat
int check_pth(pantilthatns::pantilthat *pth);

// Move servo to a desired angle
int move_servo(pantilthatns::pantilthat *pth, size_t servo_id, float angle);

// Apply an angle with PTH according to the relative position of the target
struct pos apply_angle(pantilthatns::pantilthat *pth, struct pos relative_pos);
