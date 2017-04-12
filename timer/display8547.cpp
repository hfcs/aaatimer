#include <Arduino.h>
#include <ArduinoLog.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "display.h"
#include "board.h"
#include "timer_event.h"
#include "utils.h"

#if (DISPLAY_CONFIG == DISPLAY_PCF8574_LCD1602) || (DISPLAY_CONFIG == DISPLAY_PCF8574_LCD2004)

// Implements I2C LCD over 8574/8574A
// Currently support 16 character x 2 row (1602)
// Assuming 8574 LCD is the only I2C device attached

static LiquidCrystal_PCF8574 *lcd = NULL;

#if defined(ACTCAM_SUPPORT)
#define WIFI_ON_ICON    (0)
#define WIFI_OFF_ICON   (1)

static byte wifiOn[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

static byte wifiOff[8] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000,
};
static bool wifiConnected = false;

static void writeWifiState() {
  lcd->setCursor(15, 0);
  if (wifiConnected) {
    lcd->write(byte(WIFI_ON_ICON));
  } else {
    lcd->write(byte(WIFI_OFF_ICON));
  }
}

static void handleDisplayWifiConnected(int event, int state) {
  wifiConnected = state;
  writeWifiState();
}
#endif //defined(ACTCAM_SUPPORT)

static void writeFirstRow(String string) {
  lcd->setCursor(0, 0);
  lcd->print(string);
#if defined(ACTCAM_SUPPORT)
  writeWifiState();
#endif
}

static void writeSecondRow(String string) {
  lcd->setCursor(0, 1);
  lcd->print(string);
}

static int totalHits;
static int hit;
static unsigned long timeMillis;

static void handleDisplayclear(int event, int param) {
  timeMillis = 0;
  totalHits = 0;
  hit = 0;
  lcd->clear();
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

// This auto detection allow us hooking up various 8574/8574A LCD sourced, that
// usually shipped randomly configured
static void detectI2cDisplay() {
  byte error, address;
  Wire.begin();
  for (address = 0x20; address <= 0x27; address++) {
    // 8574  Address range is 0x20-0x27
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
      break;
  }
  if (error != 0) {
    for (address = 0x38; address <= 0x3f; address++) {
      // 8574A  Address range is 0x38-0x3f
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0)
        break;
    }
  }
  if (error != 0) {
    Log.fatal("LCD 1602 on I2C not found" CR);
    stopSketchLoop();
  } else {
    Log.notice("I2C detected on 0x%x, setting up LCD here" CR, address);
    lcd = new LiquidCrystal_PCF8574(address);
  }
}

void errorDisplay() {
  if (lcd) { // I2C failure goes here too, check that first
    writeFirstRow("Fatal error, see");
    writeSecondRow("log on serial");
  }
}

void setupDisplay() {
  detectI2cDisplay();

#if defined(ACTCAM_SUPPORT)
  lcd->createChar(WIFI_ON_ICON, wifiOn);
  lcd->createChar(WIFI_OFF_ICON, wifiOff);
#endif

#if (DISPLAY_CONFIG == DISPLAY_PCF8574_LCD1602)
  lcd->begin(16, 2); // initialize the lcd
#elif (DISPLAY_CONFIG == DISPLAY_PCF8574_LCD2004)
  lcd->begin(20, 4); // initialize the lcd
#else
  #error // invalid display config
#endif
  lcd->setBacklight(255);
  lcd->home();
  lcd->clear();

  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayClear, handleDisplayclear);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayStopwatchParamMillis, handleDisplayStopwatchParamMillis);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayShotcountParam, handleDisplayShotcountParam);
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayRefreshParamMode, handleDisplayRefreshParamMode);
#if defined(ACTCAM_SUPPORT)
  TimerEvent::getInstance()->addListener(TimerEvent::eventDisplayWifiConnected, handleDisplayWifiConnected);
#endif

  writeFirstRow("Press review");
  writeSecondRow("then press start");
}

void loopDisplay() {
  // Nothing to do now
}

#endif // DISPLAY_CONFIG
