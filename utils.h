#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

String millisToTimeString(unsigned long);
void stopSketchLoop();
boolean isSketchLoopRun();

void displayHit(int, unsigned long);
void displayReview(int, int, unsigned long);
void displayTiming();
void displayCountdown(unsigned long);

#endif // UTILS_H
