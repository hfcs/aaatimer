#include <Arduino.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <AsyncPrinter.h>
#include "actcam.h"
#include "board.h"
#include "timer_event.h"

#if defined(ACTCAM_SUPPORT)
#if defined(ACTCAM_WIFI_ESP8266)

// ESP8266 Wifi implementation

// A dumb implementation where HTTP request executed and finished one at a time,
// everything are stored as just one copy of static global.
// This design assume a chain of HTTP commands using callback based async.
static AsyncPrinter *client = NULL;
static WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
static bool wifiConnected = false;
static unsigned int respondCode;
static String respondContent;
static actionCamHttpCmdCb httpCallback;

// HTTP respondse callback is triggered by event instead of directly called
// from client->onData().
// Such decoupling simplify client usage being created/destroyed over async
// callback based HTTP reuquest chain, saving us holding multiple clients or
// needing to destroy client from its own handler (latter is unlikely feasible).
static void handleHttpRespond(int event, int param) {
  httpCallback(respondCode, respondContent);
}

void actionCamWifiConnectAp(char* ap, char* passphrase) {
  TimerEvent::getInstance()->addListener(TimerEvent::eventHttpRespond, handleHttpRespond);
  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
    wifiConnected = false;
    Log.warning("Station disconnected" CR);
  });
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
    wifiConnected = true;
    Log.warning("Station connected, IP: %s, gateway %s" CR, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str());
  });
  WiFi.begin(ap, passphrase);
}

void actionCamHttpCmdExpects200(char* host, char* path, actionCamHttpCmdCb cb) {
  unsigned long startMillis = millis();

  if (!wifiConnected) {
    Log.error("[Wifi] HTTP attempt on disconnected WiFi" CR);
    cb(503, String(""));
    return;
  }

  if (client) { // clean up last client
    delete client;
  }
  client = new AsyncPrinter();
  client->onData([](void *obj, AsyncPrinter* c, void *data, size_t len){
    char buf[len+1];
    strncpy(buf, (const char*)data, len);
    buf[len] = '\0';
    Log.notice("HTTP response: %s" CR, buf);

    String dataString = String("") + buf;
    if (dataString.startsWith("HTTP/1.")) {
      respondCode = dataString.substring(9, dataString.indexOf(' ', 9)).toInt();
      Log.notice("HTTP response code: %d" CR, respondCode);
    } else {
      respondContent = dataString;
      Log.notice("HTTP response content: %s" CR, dataString.c_str());
      TimerEvent::getInstance()->queueSoftwareEvent(TimerEvent::eventHttpRespond, 0);
    }
  }, (void*) &client);

  if(client->connect("192.168.1.254", 80) == 0) {
    Log.error("[Wifi] connect attempt failed upon HTTP request" CR);
    wifiConnected = false;
    cb(503, String(""));
  } else {
    Log.notice("[Wifi] connect OK" CR);
    respondCode = 0;
    respondContent = "";
    String httpreq = String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n";
    // see handleHttpRespond() comments for async callback triggered by event.
    httpCallback = cb;
    client->write((const uint8_t*)httpreq.c_str(), httpreq.length());
    Log.notice("[Wifi] http request send: %l ms" CR, millis() - startMillis);
  }
}

#endif //defined(ACTCAM_WIFI_ESP8266)
#endif //defined(ACTCAM_SUPPORT)
