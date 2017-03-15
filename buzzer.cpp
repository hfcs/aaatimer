#include <Arduino.h>
#include <RBD_Timer.h>
#include "timer_event.h"
#include "buzzer.h"

RBD::Timer buzzerTimer;

static void startingSound(int event, int param) {
  Serial.println("TODO: buzzer sound");
  buzzerTimer.setTimeout(1000);
  buzzerTimer.restart();
#if 0 //uncomment for round trip latency test
  TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopPlateHit, 0); // route hardwire buzzer to stop plate event for round trip latency test
#endif
}

void setupBuzzer() {
  buzzerTimer.stop();
  TimerEvent::getInstance()->addListener(TimerEvent::eventStartingSound, startingSound);
}

void handleBuzzer() {
  if (buzzerTimer.isExpired()) {
    buzzerTimer.stop();
    Serial.println("TODO: buzzer stop sound");
  }
}
