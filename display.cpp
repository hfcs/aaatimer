#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "display.h"
#include "timer_event.h"
#include "utils.h"

LiquidCrystal_PCF8574 lcd(0x3f);  // set the LCD address to 0x27 for a 16 chars and 2 line display

static void writeFirstRow(String string) {
  lcd.setCursor(0, 0);
  lcd.print(string);
}

static void writeSecondRow(String string) {
  lcd.setCursor(0, 1);
  lcd.print(string);
}

static int totalHits;
static int hit;
static unsigned long timeMillis;

static void handleDisplayclear(int event, int param) {
  timeMillis = 0;
  totalHits = 0;
  hit = 0;
  lcd.clear();
}

static void handleDisplayStopwatchParamMillis(int event, int millisParam) {
  timeMillis = (unsigned long) millisParam;
}

static void handleDisplayShotcountParam(int event, int shotsParam) {
  totalHits = (((unsigned int)shotsParam) & 0xff00) >> 8;
  hit = (int)((unsigned int)shotsParam & 0xff);
}

static void handleDisplayRefreshParamMode(int event, int modeParam) {
  if (modeParam == TimerEvent::displayModeHit) {
    String shotCount = "Hit " + String(totalHits);
    writeFirstRow(shotCount);
  } else if (modeParam == TimerEvent::displayModeReview) {
    String shotCount = "Review " + String(hit) + " of " + String(totalHits);
    writeFirstRow(shotCount);
  } else if (modeParam == TimerEvent::displayModeTiming) {
    writeFirstRow("Waiting for hit");
  } else if (modeParam == TimerEvent::displayModeCountdown) {
    writeFirstRow("Count down");
  }
  writeSecondRow(millisToTimeString(timeMillis));
}

void setupDisplay() {
  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();

  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayClear, handleDisplayclear);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayStopwatchParamMillis, handleDisplayStopwatchParamMillis);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayShotcountParam, handleDisplayShotcountParam);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayRefreshParamMode, handleDisplayRefreshParamMode);

  writeFirstRow("Press review");
  writeSecondRow("then press start");
}

void loopDisplay() {
  // Nothing to do now
}

/*
displayModeCountdown,
displayModeTiming,
displayModeHit,
displayModeReview
*/
