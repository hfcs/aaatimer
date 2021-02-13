from m5stack import *
from m5ui import *
from uiflow import *
import time

setScreenColor(0xcccccc)


timeMilli = None
countdownMilli = None
startSignalTicks = None
shotsMilli = None


title = M5Title(title="Shot Count Down Timer", x=3, fgcolor=0xFFFFFF, bgcolor=0xff0035)
labelTimer = M5TextBox(80, 125, "0.00", lcd.FONT_DejaVu72, 0x000000, rotate=0)
labelReset = M5TextBox(39, 213, "Reset", lcd.FONT_DejaVu18, 0x000000, rotate=0)
labelStart = M5TextBox(223, 213, "Start", lcd.FONT_DejaVu18, 0x000000, rotate=0)
labelShots = M5TextBox(10, 26, "shots:", lcd.FONT_Default, 0x000000, rotate=0)

import random
import math


# Refresh screen with timer reading, parameter in milliseconds
def refreshTimer(timeMilli):
  global countdownMilli, startSignalTicks, shotsMilli
  labelTimer.setText(str("%.2f"%((timeMilli / 1000))))

# Print the list of recorded shots. Input: the global list
def refreshShots():
  global timeMilli, countdownMilli, startSignalTicks, shotsMilli
  labelShots.setText(str((str('shots:') + str(str(shotsMilli)))))

# Timer reset logic
def resetCountDown():
  global timeMilli, countdownMilli, startSignalTicks, shotsMilli
  startSignalTicks = 0
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  refreshTimer(0)

# Timer start logic
def startCountDown(countdownMilli):
  global timeMilli, startSignalTicks, shotsMilli
  if startSignalTicks == 0:
    startSignalTicks = (time.ticks_ms()) + countdownMilli
    timerSch.run('timerCountDown', countdownMilli, 0x01)
    timerSch.run('timeRefreshTick', 57, 0x00)
    shotsMilli = []
    refreshShots()


def buttonA_wasPressed():
  global startSignalTicks, timeMilli, shotsMilli, countdownMilli
  resetCountDown()
  pass
btnA.wasPressed(buttonA_wasPressed)

def buttonC_wasPressed():
  global startSignalTicks, timeMilli, shotsMilli, countdownMilli
  startCountDown(random.randint(1000, 4000))
  pass
btnC.wasPressed(buttonC_wasPressed)

def buttonB_wasPressed():
  global startSignalTicks, timeMilli, shotsMilli, countdownMilli
  timerSch.stop('timeRefreshTick')
  refreshTimer(math.fabs(startSignalTicks - (time.ticks_ms())))
  if len(shotsMilli) <= 15:
    shotsMilli.insert(0, "%.2f"%((((time.ticks_ms()) - startSignalTicks) / 1000)))
    refreshShots()
  pass
btnB.wasPressed(buttonB_wasPressed)

@timerSch.event('timeRefreshTick')
def ttimeRefreshTick():
  global startSignalTicks, timeMilli, shotsMilli, countdownMilli
  refreshTimer(math.fabs(startSignalTicks - (time.ticks_ms())))
  pass

@timerSch.event('timerCountDown')
def ttimerCountDown():
  global startSignalTicks, timeMilli, shotsMilli, countdownMilli
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  speaker.sing(889, 1)
  timerSch.run('timeRefreshTick', 227, 0x00)
  pass


lcd.clear()
title.show()
labelTimer.show()
labelReset.show()
labelStart.show()
labelShots.show()
while True:
  wait_ms(2)
