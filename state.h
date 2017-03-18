#ifndef STATE_H
#define STATE_H

#include "timer_event.h"

typedef enum {
  TIMER_STATE_RESET,
  TIMER_STATE_COUNTDOWN,
  TIMER_STATE_TIMING,
  TIMER_STATE_HIT
} TIMER_STATE;

extern TIMER_STATE timer_state;

void setupState();
void loopState();
void updateStateMachine(TimerEvent::EventType event);

#endif // STATE_H
