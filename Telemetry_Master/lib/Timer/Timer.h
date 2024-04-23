#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:

    Timer(uint32_t interval);

    uint32_t num_loops;

    void reset();

    bool check();

private:
    uint32_t interval;  
    uint32_t lastResetTime;  
    uint16_t period_millis;
    uint32_t last_loop_millis;
    bool already_checked;
};

#endif
