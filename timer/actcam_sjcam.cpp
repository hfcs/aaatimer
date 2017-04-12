#include <Arduino.h>
#include <ArduinoLog.h>
#include "actcam.h"
#include "board.h"

#if defined(ACTCAM_SUPPORT)

// SJCAM commands implementation, tested on SJ4000

bool actionCamInitCmd() {

}

/* SJCam sample response

HTTP response: HTTP/1.1 200 OK
Server: eCos/1.0
Cache-Control:no-store, no-cache, must-revalidate
Pragma: no-cache
Accept-Ranges: bytes
Content-length: 97
Content-type: text/xml
Connection: close

HTTP response: <?xml version="1.0" encoding="UTF-8" ?>
<Function>
<Cmd>2001</Cmd>
<Status>0</Status>
</Function>

*/

static int parseValueFromXmlContent(String content) {
  int start = content.indexOf("<Value>") + strlen("<Value>");
  int end = content.indexOf("</Value>");
  return content.substring(start, end).toInt();
}

void actionCamRecStartCmd(actionCamRecStartCmdCb cb) {
  actionCamHttpCmdExpects200((char*)"192.168.1.254", (char*)"/?custom=1&cmd=2001&par=1", [=](unsigned int code, String content) {
    if (code == 200) {
      cb(true);
    } else {
      cb(false);
    }
  });
}

void actionCamRecStopCmd(actionCamRecStopCmdCb cb) {
  actionCamHttpCmdExpects200((char*)"192.168.1.254", (char*)"/?custom=1&cmd=2001&par=0", [=](unsigned int code, String content) {
    if (code == 200) {
      cb(true);
    } else {
      cb(false);
    }
  });
}

void actionCamIsRecordingCmd(actionCamIsRecordingCmdCb cb) {
  actionCamHttpCmdExpects200((char*)"192.168.1.254", (char*)"/?custom=1&cmd=2016", [=](unsigned int code, String content) {
    if (code == 200) {
      if(parseValueFromXmlContent(content) == 0) {
        cb (false);
      } else {
        cb (true);
      }
    } else {
      Log.error("[Actcam] camera status query failed" CR);
      cb(false); // assume is recording if we failed
    }
  });
}

#endif //defined(ACTCAM_SUPPORT)
