#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#define DISPLAY_PCF8574_LCD1602   (1)
#define DISPLAY_PCF8574_LCD2004   (2)
#define DISPLAY_SSD1306           (3)
#define DISPLAY_SH1106            (4)

void errorDisplay();
void setupDisplay();
void loopDisplay();

#endif // DISPLAY_H
