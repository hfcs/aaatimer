#include <Arduino.h>
#include <EventManager.h>
#include "timer_event.h"
#include "board.h"

// Global static pointer used to ensure a single instance of the class.
TimerEvent* TimerEvent::instance = NULL;

static void defaultListener(int event, int param) {
  Serial.print("TODO: Unhandled event, ID = ");
  Serial.println(event);
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
  timerEventManager.addListener(eventCode, listener);
}

void TimerEvent::queueHardwareEvent(TimerEvent::EventType event, int param) {
  timerEventManager.queueEvent(event, param, EventManager::kHighPriority);
}

void TimerEvent::queueSoftwareEvent(TimerEvent::EventType event, int param) {
  timerEventManager.queueEvent(event, param, EventManager::kLowPriority);
}

void TimerEvent::processEvent() {
  timerEventManager.processEvent();
}
