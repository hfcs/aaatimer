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
    hardwareLedOff,
    hardwareBuzzerOn,
    hardwareBuzzerOff,
    hardwareStopwatchReset,
    hardwareStopwatchStart,
    hardwareStopwatchRecordHit,

    // software events usually takes more complex processing and lower priority
    softwareReset = 200,
    softwareStartCountDown,
    softwareCountDownExpire,
    softwareStartingSound
  };
  void setupTimerEvent();
  void addListener(int, EventManager::EventListener);
  void queueHardwareEvent(TimerEvent::EventType, int);
  void queueSoftwareEvent(TimerEvent::EventType, int);
  void processEvent();
  boolean isListenerListFull();
};

#endif // TIMER_EVENT_H
