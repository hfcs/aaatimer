#include <Arduino.h>
#include "timer_event.h"
#include "buzzer.h"

static void startingSound(int event, int param) {
  Serial.println("TODO: buzzer sound");
#if 0 //uncomment for round trip latency test
  TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopPlateHit, 0); // route hardwire buzzer to stop plate event for round trip latency test
#endif
}

void setupBuzzer() {
  TimerEvent::getInstance()->addListener(TimerEvent::eventStartingSound, startingSound);
}

void handleBuzzer() {
  // Nothing to do here as an event sink
}
