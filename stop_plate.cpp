#include <Arduino.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>
#include "board.h"
#include "timer_event.h"
#include "stop_plate.h"

// All button functions encapsulate here
// TODO: interrupt driven, debounced button firing events

RBD::Button stopPlate(STOP_PLATE_PIN);

void setupStopPlate() {
}

void handleStopPlate() {
  if(stopPlate.onPressed()) {
    queueHardwareEvent(TimerEvent::hardwareStopPlateHit, 0);
  }
}
