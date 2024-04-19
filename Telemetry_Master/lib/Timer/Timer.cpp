#include "Timer.h"

Timer::Timer(uint32_t interval)
    : interval(interval), lastResetTime(0) {
    reset();
}


void Timer::reset() {
    lastResetTime = millis();
}


bool Timer::check()
{
  if (millis() - last_loop_millis >= period_millis && !already_checked)
  {
    already_checked = true;
    num_loops++;
    return true;
  }
  return false;
}
