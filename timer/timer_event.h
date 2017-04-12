#ifndef TIMER_EVENT_H
#define TIMER_EVENT_H

#include <EventManager.h>

class TimerEvent {
private:
  static TimerEvent* instance;
  EventManager timerEventManager;
  // Singleton disable constructor, copy constructor and assignment
  TimerEvent(){};
  TimerEvent(TimerEvent const& copy){};
  TimerEvent& operator=(TimerEvent const& copy){};
public:
  static TimerEvent* getInstance();
  ~TimerEvent();
  enum EventType {
    // hardware events are usually fast, could process in ISR and
    // resulting in firing software events
    hardwareReviewKey = 100,
    hardwareStartKey,
    hardwareStopPlateHit,

    // pseudo hardware events call for high priority
    hardwareStopwatchStart,
    hardwareStopwatchRecordHit,

    // software events usually takes more complex processing and lower priority
    eventResetStopPlate = 200,
    eventStartCountDown,
    eventCountDownExpire,
    eventStartingSound,
    eventActCamRecStart,
    eventActCamRecStop,

    // display events, some with parameter
    eventDisplayClear = 300,          // clear display and data
    eventDisplayStopwatchParamMillis, // set stopwatch time, parameter in millis
    eventDisplayShotcountParam,       // lower char = shot #, upper char = total shots
    eventDisplayRefreshParamMode,    // mode encoded in parameter
    eventDisplayWifiConnected,        // bool encoded in parameter

    // HTTP/Wifi handling events
    eventHttpRespond = 400,           // HTTP respond returns (e.g. can trigger callback here)
  };

  enum DisplayMode {
    displayModeCountdown,
    displayModeTiming,
    displayModeHit,
    displayModeReview
  };

  void setupTimerEvent();
  void addListener(int, EventManager::EventListener);
  void queueHardwareEvent(TimerEvent::EventType, int);
  void queueSoftwareEvent(TimerEvent::EventType, int);
  void processEvent();
  void processAllEvents();
  boolean isListenerListFull();
};

#endif // TIMER_EVENT_H
