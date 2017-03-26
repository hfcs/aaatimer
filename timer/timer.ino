#include <Arduino.h>
#include <ArduinoLog.h>
#include "board.h"
#include "timer_event.h"
#include "button.h"
#include "stop_plate.h"
#include "buzzer.h"
#include "display.h"
#include "state.h"
#include "stopwatch.h"
#include "actcam.h"
#include "utils.h"

void setup() {
  Serial.begin(115200);
  Log.begin(TIMER_LOG_LEVEL, &Serial);
  delay(100);
  setupState();
  TimerEvent::getInstance()->setupTimerEvent();
  setupButton();
  setupStopPlate();
  setupStopwatch();
  setupBuzzer();
  setupDisplay();
  setupActionCam();
  Serial.println("");
  if (TimerEvent::getInstance()->isListenerListFull()) {
    Log.fatal("too many listeners" CR);
    Log.fatal("Resize EventManager library EventManager.h EVENTMANAGER_LISTENER_LIST_SIZE" CR);
    Log.fatal("and then rebuild" CR);
    stopSketchLoop();
  }
}

void loop() {
  if (isSketchLoopRun()){
    loopButton();
    loopStopPlate();
    loopState();
    loopBuzzer();
    loopActionCam();
    TimerEvent::getInstance()->processAllEvents();
  } else {
    errorDisplay();
  }
  delay(5);
}
