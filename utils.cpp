#include <Arduino.h>

static String digitToString (unsigned long digit) {
  if (digit == 0) {
    return "00";
  }
  if (digit < 10) {
    return "0" + String(digit);
  }
  return String(digit);
}

String millisToTimeString(unsigned long elapse) {
  unsigned long minute = elapse / 1000 / 60;
  unsigned long second = (elapse % (1000 * 60)) / 1000;
  unsigned long deciSecond = (elapse % 1000) / 10;
  if ((elapse % 10) > 4) { // round to the nearest deciSecond
    deciSecond++;
  }
  return digitToString(minute) + ":" + digitToString(second) + ":" + digitToString(deciSecond);
}
