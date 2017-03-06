#include <Arduino.h>
#include "timer_event.h"
#include "led.h"

static void allLedOff(int event, int param) {
  Serial.println("TODO: all LED off");
}

static void ledOneOnLedTwoOff(int event, int param) {
  Serial.println("TODO: LED 1 on, LED 2 off");
}

static void ledOneOffLedTwoOn(int event, int param) {
  Serial.println("TODO: LED 1 off, LED 2 on");
}

void setupLed() {
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareLedOff, allLedOff);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareLedOneOnLedTwoOff, ledOneOnLedTwoOff);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareLedOneOffLedTwoOn, ledOneOffLedTwoOn);
}

void handleLed() {
  // Nothing to do here as an event sink
}
