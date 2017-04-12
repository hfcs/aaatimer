#include <Arduino.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "timer_event.h"
#include "actcam.h"
#include "board.h"

#if defined(ACTCAM_SUPPORT)
#if defined(ACTCAM_SJCAM)

// Action camera support. Supporting SJCAM API known in the web and tested on
// SJ4000 Wifi
// http://sj4000programming.sourceforge.net/

static unsigned long camStopAfterMillis;
static bool camStarted = false;

static void handleEventActCamRecStart(int event, int param) {
  actionCamIsRecordingCmd([](bool isRecording){
    if (isRecording) {
      Log.notice("[Actcam] start while recording in progress" CR);
      actionCamRecStopCmd([](bool ok){
        if (ok) {
          camStarted = false;
          Log.notice("[Actcam] stop command before restart returns OK" CR);
          actionCamRecStartCmd([](bool ok){
            if (ok) {
              camStopAfterMillis = millis() + ACTCAM_STOP_MILLIS_AFTER_START;
              camStarted = true;
              Log.notice("[Actcam] start command returns OK" CR);
            } else {
              Log.error("[Actcam] start command failed" CR);
            }
          });
        } else {
          Log.error("[Actcam] stop command before restart failed" CR);
        }
      });
    } else {
      actionCamRecStartCmd([](bool ok){
        if (ok) {
          camStopAfterMillis = millis() + ACTCAM_STOP_MILLIS_AFTER_START;
          camStarted = true;
          Log.notice("[Actcam] start command returns OK" CR);
        } else {
          Log.error("[Actcam] start command failed" CR);
        }
      });
    }
  });
}

static void handleEventActCamRecStop(int event, int param) {
  actionCamRecStopCmd([](bool ok){
    if (ok) {
      camStarted = false;
      Log.notice("[Actcam] stop command returns OK" CR);
    } else {
      Log.error("[Actcam] stop command failed" CR);
    }
  });
}

static void updateCamStopTime(int event, int param) {
  camStopAfterMillis = millis() + ACTCAM_STOP_MILLIS_AFTER_LAST_HIT;
}

void setupActionCam() {
  actionCamWifiConnectAp((char*)ACTCAM_AP_NAME, (char*)ACTCAM_AP_PASSWORD);
  TimerEvent::getInstance()->addListener(TimerEvent::eventActCamRecStart, handleEventActCamRecStart);
  TimerEvent::getInstance()->addListener(TimerEvent::eventActCamRecStop, handleEventActCamRecStop);
  TimerEvent::getInstance()->addListener(TimerEvent::hardwareStopwatchRecordHit, updateCamStopTime);
  actionCamInitCmd();
}

void loopActionCam() {
  if (camStarted) {
    if (millis() > camStopAfterMillis) {
      camStarted = false; // only fire once
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::eventActCamRecStop, 0);
    }
  }
}

#endif //#if defined(ACTCAM_SJCAM)
#endif //defined(ACTCAM_SUPPORT)
