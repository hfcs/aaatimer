#include <Arduino.h>
#include <EventManager.h>
#include "timer_event.h"
#include "board.h"

EventManager timerEventManager;

static void defaultListener(int event, int param) {
  Serial.print("TODO: Unhandled event, ID = ");
  Serial.println(event);
}

void setupTimerEvent() {
  // Hardware setup
  pinMode(ADC_PIN, INPUT);

  timerEventManager.setDefaultListener(defaultListener);

  // Add interrupts here
}

void addListener(int eventCode, EventManager::EventListener listener) {
  timerEventManager.addListener(eventCode, listener);
}

void queueHardwareEvent(TimerEvent::EventType event, int param) {
  timerEventManager.queueEvent(event, param, EventManager::kHighPriority);
}

void queueSoftwareEvent(TimerEvent::EventType event, int param) {
  timerEventManager.queueEvent(event, param, EventManager::kLowPriority);
}

void processEvent() {
  timerEventManager.processEvent();
}
