#ifndef ACTCAM_H
#define ACTCAM_H

#include <Arduino.h>
#include <functional>

void setupActionCam();
void loopActionCam();

// Assuming an action camera acts like a Wifi AP (e.g. SJCam)
// Different cameras and Wifi interface are abstracted in these interface
// functions, where implementations are provided by different .cpp file and
// #ifdef included

typedef std::function<void(bool)> actionCamIsRecordingCmdCb;
typedef std::function<void(bool)> actionCamRecStartCmdCb;
typedef std::function<void(bool)> actionCamRecStopCmdCb;
typedef std::function<void(int, String)> actionCamHttpCmdCb;

////// Wifi implmentation specific interface ///////
void actionCamWifiConnectAp(char*, char*);
void actionCamHttpCmdExpects200(char *, char *, actionCamHttpCmdCb);

////// Camera specific interface //////
bool actionCamInitCmd();   // Any start up default wanted
void actionCamRecStartCmd(actionCamRecStartCmdCb cb);
void actionCamRecStopCmd(actionCamRecStopCmdCb cb);
void actionCamIsRecordingCmd(actionCamIsRecordingCmdCb cb);

#endif // ACTCAM_H
