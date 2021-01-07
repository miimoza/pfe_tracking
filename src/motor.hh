#include "libpth/pantilthat.h"

pantilthat *get_pth();
int check_pth(pantilthat *pth);
int move_servo(pantilthat *pth, size_t servo_id, float angle);
