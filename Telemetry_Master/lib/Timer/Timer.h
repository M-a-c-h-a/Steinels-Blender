#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:

    Timer(uint32_t interval);


    void reset();

    bool check();

private:
    uint32_t interval;  
    uint32_t lastResetTime;  
};

#endif
