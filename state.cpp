#include <Arduino.h>
#include "timer_event.h"
#include "state.h"

/*

machine state
-reset
-countdown
-sounded
-led 1
-led 2

*/

TIMER_STATE timer_state;

void setupState() {
  timer_state = TIMER_STATE_RESET;
}

void updateStateMachine(TimerEvent::EventType event) {
  switch (timer_state) {
    case TIMER_STATE_RESET:
      /*
      On start event => countdown state
      */
      break;
    case TIMER_STATE_COUNTDOWN:
      /*
      On countdown expire event => sounded state
      On review event => reset state
      */
      break;
    case TIMER_STATE_SOUNDED:
      /*
      On hit event => led 1 state
      On review event => reset state
      */
      break;
    case TIMER_STATE_LED_1:
      /*
      On hit event => led 2 state
      On reset event => reset state
      On review event => show next shot
      */
      break;
    case TIMER_STATE_LED_2:
      /*
      On hit event => led 1 state
      On reset event => reset state
      On review event => show next shot
      */
      break;
  }
}
