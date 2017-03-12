#include <Arduino.h>
#include "timer_event.h"
#include "buzzer.h"

static void startingSound(int event, int param) {
  Serial.println("TODO: buzzer sound");
}

void setupBuzzer() {
  TimerEvent::getInstance()->addListener(TimerEvent::eventStartingSound, startingSound);
}

void handleBuzzer() {
  // Nothing to do here as an event sink
}
