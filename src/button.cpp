#include <RBD_Timer.h>
#include <RBD_Button.h>
#include "button.h"
#include "board.h"
#include "timer_event.h"

// All button functions encapsulate here
// TODO: interrupt driven, debounced button firing events

static RBD::Button buttonStart(START_PIN);
static RBD::Button buttonReset(REVIEW_PIN);

void setupButton() {
  // Nothing here for this implementation because
  // 1. Pin setup is done in declaration
  // 2. This is an event producer, not event consider. Listener function is elsewhere
}

void loopButton() {
  // prioritize reset over start as former always overrule latter
  if(buttonReset.onPressed()) {
    TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareReviewKey, 0);
  } else if (buttonStart.onPressed()) {
    TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStartKey, 0);
  }
}
