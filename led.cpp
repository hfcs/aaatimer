#include <Arduino.h>
#include "timer_event.h"
#include "led.h"

static void allLedOff(int event, int param) {
  Serial.println("TODO: all LED off");
}

static void ledOnAndToggle(int event, int param) {
  Serial.println("TODO: LED on, toggle if needed");
}

void setupLed() {
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareLedOff, allLedOff);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, ledOnAndToggle);
}

void handleLed() {
  // Nothing to do here as an event sink
}
