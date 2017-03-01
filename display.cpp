#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "display.h"

LiquidCrystal_PCF8574 lcd(0x3f);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setupDisplay() {
  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("LCD OK");
}

void writeFirstRow(String string) {
  lcd.setCursor(0, 0);
  lcd.print(string);
}

void writeSecondRow(String string) {
  lcd.setCursor(0, 1);
  lcd.print(string);
}
