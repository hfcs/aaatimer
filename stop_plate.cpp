#include <Arduino.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>
#include "board.h"
#include "timer_event.h"
#include "stop_plate.h"

// All button functions encapsulate here
// TODO: interrupt driven, debounced button firing events

RBD::Button stopPlate(STOP_PLATE_PIN);

static void allLedOff(int event, int param) {
  Serial.println("TODO: all LED off");
}

static void ledOnAndToggle(int event, int param) {
  Serial.println("TODO: LED on, toggle if needed");
}

void setupStopPlate() {
  TimerEvent::getInstance()->addListener(TimerEvent::eventResetStopPlate, allLedOff);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, ledOnAndToggle);
}

void handleStopPlate() {
  if(stopPlate.onPressed()) {
    TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopPlateHit, 0);
  }
}
