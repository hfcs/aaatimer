#include <Arduino.h>
#include "stopwatch.h"
#include "display.h"
#include "utils.h"

// All timestamps are in millis as it overflows in 50 days, more than enough
// for running a shooting match

unsigned long cofStartMillis; // millis for course fire start, AKA starting beep
unsigned long cofStopplateMillis[4 * 28]; // for practical purpose, someone is not gonna dump 4 magazines of open diviion into the stop plate
unsigned long currentShotIndex;

void updateStopwatchDisplay() {
  unsigned long cofElapseMillis = millis() - cofStartMillis;
  writeSecondRow(millisToTimeString(cofElapseMillis));
}
