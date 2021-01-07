#include "utils.hh"

#include <cerrno>
#include <iostream>
#include <ostream>
#include <time.h>

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
        std::cerr << __FILE__ << ":" << __LINE__ << ":" << __func__
                  << " nanosleep() failed" << std::endl;
    }
    return result;
}
