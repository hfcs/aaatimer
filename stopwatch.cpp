#include <Arduino.h>
#include "stopwatch.h"
#include "timer_event.h"
#include "display.h"
#include "utils.h"
#include "board.h"

// All timestamps are in millis as it overflows in 50 days, more than enough
// for running a shooting match

#define MAX_HITS (4 * 28) // for practical purpose, someone is not gonna dump 4 magazines of open diviion into the stop plate

unsigned long cofStartMillis; // millis for course fire start, AKA starting beep
unsigned long cofStopplateMillis[MAX_HITS];
unsigned int currentShotIndex;
unsigned int reviewIndex; // review is handled here for handiness next to hit data

void startStopwatch(int event, int param) {
  int i;
  for (i = 0; i < MAX_HITS; i++) {
    cofStopplateMillis[i] = 0;
  }
  currentShotIndex = 0;
  reviewIndex = 0;
  cofStartMillis = millis();
  displayTiming();
}

void recordHit(int event, int param) {
  if (currentShotIndex < MAX_HITS) {
    unsigned long delta = millis() - cofStartMillis;
    Serial.print("DEBUG: hit");
    Serial.print(currentShotIndex + 1);
    Serial.print(" in raw millis ");
    Serial.println(delta);

    cofStopplateMillis[currentShotIndex] = millis() - cofStartMillis - ROUND_TRIP_LATENCY; // shot time is adjusted for end to end round trip latency
    displayHit(currentShotIndex + 1, cofStopplateMillis[currentShotIndex]);
    reviewIndex = ++currentShotIndex;
  }
  // TODO: update screen to latest hit
}

void handleReview(int event, int param) {
  if (reviewIndex <= 1) {
    reviewIndex = currentShotIndex;
  } else {
    reviewIndex--;
  }
  displayReview(reviewIndex, currentShotIndex, cofStopplateMillis[reviewIndex - 1]);
}

void setupStopwatch() {
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchStart, startStopwatch);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, recordHit);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareReviewKey, handleReview);
}
