#include <Arduino.h>
#include <EventManager.h>
#include <ArduinoLog.h>
#include "timer_event.h"
#include "board.h"

// Global static pointer used to ensure a single instance of the class.
TimerEvent* TimerEvent::instance = NULL;

static void defaultListener(int event, int param) {
  Log.error("TODO: Unhandled event, ID = %d" CR, event);
}

TimerEvent* TimerEvent::getInstance()
{
  if(!instance) {
    instance = new TimerEvent();
  }
  return instance;
}

void TimerEvent::setupTimerEvent() {
  // Hardware setup
  pinMode(ADC_PIN, INPUT);

  timerEventManager.setDefaultListener(defaultListener);

  // Add interrupts here
}

void TimerEvent::addListener(int eventCode, EventManager::EventListener listener) {
  Log.notice("addListener(), event type: %d" CR, eventCode);
  timerEventManager.addListener(eventCode, listener);
}

void TimerEvent::queueHardwareEvent(TimerEvent::EventType event, int param) {
  Log.notice("queueHardwareEvent(), event type: %d, param: %d" CR, event, param);
  timerEventManager.queueEvent(event, param, EventManager::kHighPriority);
}

void TimerEvent::queueSoftwareEvent(TimerEvent::EventType event, int param) {
  Log.notice("queueSoftwareEvent(), event type: %d, param: %d" CR, event, param);
  timerEventManager.queueEvent(event, param, EventManager::kLowPriority);
}

void TimerEvent::processEvent() {
#if (TIMER_LOG_LEVEL >= LOG_LEVEL_NOTICE) // only when needed
  int queueSize = timerEventManager.getNumEventsInQueue();
  if (queueSize != 0) { // avoid excessive log from loop()
    Log.notice("processEvent() on queue size %d" CR, queueSize);
  }
#endif
  timerEventManager.processEvent();
}

void TimerEvent::processAllEvents() {
#if (TIMER_LOG_LEVEL >= LOG_LEVEL_NOTICE) // only when needed
  int queueSize = timerEventManager.getNumEventsInQueue();
  if (queueSize != 0) { // avoid excessive log from loop()
    Log.notice("proccessAllEvents() on queue size %d" CR, queueSize);
  }
#endif
  timerEventManager.processAllEvents();
}

boolean TimerEvent::isListenerListFull() {
  return timerEventManager.isListenerListFull();
}
