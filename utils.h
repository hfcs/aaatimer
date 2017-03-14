#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

String millisToTimeString(unsigned long);

void displayHit(int, unsigned long);
void displayReview(int, int, unsigned long);
void displayTiming();

#endif // UTILS_H
