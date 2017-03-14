#include <Arduino.h>
#include "stopwatch.h"
#include "timer_event.h"
#include "display.h"
#include "utils.h"

// All timestamps are in millis as it overflows in 50 days, more than enough
// for running a shooting match

#define MAX_HITS (4 * 28) // for practical purpose, someone is not gonna dump 4 magazines of open diviion into the stop plate

unsigned long cofStartMillis; // millis for course fire start, AKA starting beep
unsigned long cofStopplateMillis[MAX_HITS];
unsigned long currentShotIndex;

void updateStopwatchDisplay() {
  unsigned long cofElapseMillis = millis() - cofStartMillis;
  writeSecondRow(millisToTimeString(cofElapseMillis));
}

void startStopwatch(int event, int param) {
  int i;
  for (i = 0; i < MAX_HITS; i++) {
    cofStopplateMillis[i] = 0;
  }
  currentShotIndex = 0;
  cofStartMillis = millis();
}

void recordHit(int event, int param) {
  if (currentShotIndex < MAX_HITS) {
    cofStopplateMillis[currentShotIndex++] = millis() - cofStartMillis;
#if 0 //uncomment for round trip latency test
    {
      unsigned long delta = millis() - cofStartMillis;
      Serial.println(millisToTimeString(delta));
      Serial.println(delta);
    }
#endif
  }
  // TODO: update screen to latest hit
}

void setupStopwatch() {
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchStart, startStopwatch);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, recordHit);
}
