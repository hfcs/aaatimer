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

void stateResetHandleEvents(int event, int param) {
  if (timer_state == TIMER_STATE_RESET) {
    if (event == TimerEvent::hardwareStartKey) {
      timer_state = TIMER_STATE_COUNTDOWN;
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareStartCountDown, 0);
    }
    /*
    On start event => countdown state
    */
  }
}

void stateCountdownHandleEvents(int event, int param) {
  if (timer_state == TIMER_STATE_COUNTDOWN) {
    if (event == TimerEvent::hardwareReviewKey) {
      timer_state = TIMER_STATE_RESET;
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareReset, 0);
    } else if (event = TimerEvent::softwareCountDownExpire) {
      timer_state = TIMER_STATE_TIMING;
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopwatchStart, 0);
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareStartingSound, 0);
    }
    /*
    On countdown expire event => sounded state
    On review event => reset state
    */
  }
}

void stateTimingHandleEvents(int event, int param) {
  if (timer_state == TIMER_STATE_TIMING) {
    if (event == TimerEvent::hardwareStopPlateHit) {
      timer_state = TIMER_STATE_LED_1;
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopwatchRecordHit, 0);
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareLedOneOnLedTwoOff, 0);
    } else if (event == TimerEvent::hardwareReviewKey) {
      timer_state = TIMER_STATE_RESET;
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareReset, 0);
    }
    /*
    On hit event => led 1 state
    On review event => reset state
    */
  }
}

void stateLed1HandleEvents(int event, int param) {
  if (timer_state == TIMER_STATE_LED_1) {
    if (event == TimerEvent::hardwareStopPlateHit) {
      timer_state = TIMER_STATE_LED_2;
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopwatchRecordHit, 0);
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareLedOneOffLedTwoOn, 0);
    } else if (event == TimerEvent::hardwareReviewKey) {
      timer_state = TIMER_STATE_RESET;
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareReset, 0);
    }
    /*
    On hit event => led 2 state
    On reset event => reset state
    On review event => show next shot
    */
  }
}

void stateLed2HandleEvents(int event, int param) {
  if (timer_state == TIMER_STATE_LED_2) {
    if (event == TimerEvent::hardwareStopPlateHit) {
      timer_state = TIMER_STATE_LED_1;
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopwatchRecordHit, 0);
      TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareLedOneOnLedTwoOff, 0);
    } else if (event == TimerEvent::hardwareReviewKey) {
      timer_state = TIMER_STATE_RESET;
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareReset, 0);
    }
    /*
    On hit event => led 1 state
    On reset event => reset state
    On review event => show next shot
    */
  }
}

// Everything needed to reset a timer
void handleSoftwareResetEvent(int event, int param) {
  TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopwatchReset, 0);
  TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareLedOff, 0);
}

void handleStartCountDown(int event, int param) {
  TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::softwareCountDownExpire, 0);
  //TODO: counter down setup
}

void handleCountDownExpire(int event, int param) {
  //TODO: state change taken care of that
}

void setupState() {
  timer_state = TIMER_STATE_RESET;
  // State changing events handlers
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStartKey, stateResetHandleEvents);

  TimerEvent::getInstance()->addListener(TimerEvent::hardwareReviewKey, stateCountdownHandleEvents);
  TimerEvent::getInstance()->addListener(TimerEvent::softwareCountDownExpire, stateCountdownHandleEvents);

  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopPlateHit, stateTimingHandleEvents);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareReviewKey, stateTimingHandleEvents);

  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopPlateHit, stateLed1HandleEvents);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareReviewKey, stateLed1HandleEvents);

  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopPlateHit, stateLed2HandleEvents);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareReviewKey, stateLed2HandleEvents);

  // Software event handlers
  TimerEvent::getInstance()->addListener(TimerEvent::softwareReset, handleSoftwareResetEvent);
  TimerEvent::getInstance()->addListener(TimerEvent::softwareStartCountDown, handleStartCountDown);
  TimerEvent::getInstance()->addListener(TimerEvent::softwareCountDownExpire, handleCountDownExpire);
}
