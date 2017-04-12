#ifndef STATE_H
#define STATE_H

#include "timer_event.h"

typedef enum {
  TIMER_STATE_RESET,
  TIMER_STATE_COUNTDOWN,
  TIMER_STATE_TIMING,
  TIMER_STATE_HIT
} TIMER_STATE;

void setupState();
void loopState();

#endif // STATE_H
