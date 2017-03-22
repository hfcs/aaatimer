#include <Arduino.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>
#include <ArduinoLog.h>
#include "board.h"
#include "timer_event.h"
#include "stop_plate.h"

// All button functions encapsulate here
// TODO: interrupt driven, debounced button firing events

static RBD::Button stopPlate(STOP_PLATE_PIN);

static void allLedOff(int event, int param) {
  Log.warning("TODO: all LED off" CR);
}

static void ledOnAndToggle(int event, int param) {
  Log.warning("TODO: LED on, toggle if needed" CR);
}

void setupStopPlate() {
  stopPlate.setDebounceTimeout(0); // volume sensor can skipp debouncing
  TimerEvent::getInstance()->addListener(TimerEvent::eventResetStopPlate, allLedOff);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, ledOnAndToggle);
}

void loopStopPlate() {
  if(stopPlate.onPressed()) {
    TimerEvent::getInstance()->queueHardwareEvent(TimerEvent::hardwareStopPlateHit, 0);
  }
}
