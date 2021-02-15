from m5stack import *
from m5ui import *
from uiflow import *
import time

setScreenColor(0xffffff)


timeMilli = None
countdownMilli = None
shotTimeMilli = None
startSignalTicks = None
shotTimeListMilli = None


title = M5Title(title="Shot Count Down Timer", x=3, fgcolor=0xFFFFFF, bgcolor=0xff0035)
labelTimer = M5TextBox(80, 125, "0.00", lcd.FONT_DejaVu72, 0x000000, rotate=0)
labelReset = M5TextBox(39, 213, "Reset", lcd.FONT_DejaVu18, 0x000000, rotate=0)
labelStart = M5TextBox(226, 213, "Start", lcd.FONT_DejaVu18, 0x000000, rotate=0)
logoImage = M5Img(120, 196, "res/logo.png", True)
labelShots = M5TextBox(10, 26, "split:", lcd.FONT_Default, 0x000000, rotate=0)

import random
import math


# Refresh screen with timer reading, parameter in milliseconds
def refreshTimer(timeMilli):
  global countdownMilli, shotTimeMilli, startSignalTicks, shotTimeListMilli
  labelTimer.setText(str("%.2f"%((timeMilli / 1000))))

# Print the list of recorded shots. Input: the global list
def refreshShots():
  global timeMilli, countdownMilli, shotTimeMilli, startSignalTicks, shotTimeListMilli
  labelShots.setText(str((str('shots:') + str(str(shotTimeListMilli)))))

# Timer reset logic
def resetCountDown():
  global timeMilli, countdownMilli, shotTimeMilli, startSignalTicks, shotTimeListMilli
  startSignalTicks = 0
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  refreshTimer(0)

# Timer start logic
def startCountDown(countdownMilli):
  global timeMilli, shotTimeMilli, startSignalTicks, shotTimeListMilli
  if startSignalTicks == 0:
    startSignalTicks = (time.ticks_ms()) + countdownMilli
    timerSch.run('timerCountDown', countdownMilli, 0x01)
    timerSch.run('timeRefreshTick', 57, 0x00)
    shotTimeListMilli = []
    refreshShots()

# Handle the shot including timer update, and previous shot time recording
def recordShot(shotTimeMilli):
  global timeMilli, countdownMilli, startSignalTicks, shotTimeListMilli
  if len(shotTimeListMilli) >= 20:
    shotTimeListMilli.pop()
  shotTimeListMilli.insert(0, "%.2f"%((shotTimeMilli / 1000)))
  refreshTimer(shotTimeMilli)
  refreshShots()


def buttonA_wasPressed():
  global startSignalTicks, shotTimeListMilli, timeMilli, countdownMilli, shotTimeMilli
  resetCountDown()
  pass
btnA.wasPressed(buttonA_wasPressed)

def buttonC_wasPressed():
  global startSignalTicks, shotTimeListMilli, timeMilli, countdownMilli, shotTimeMilli
  startCountDown(random.randint(1000, 4000))
  pass
btnC.wasPressed(buttonC_wasPressed)

def buttonB_wasPressed():
  global startSignalTicks, shotTimeListMilli, timeMilli, countdownMilli, shotTimeMilli
  timerSch.stop('timeRefreshTick')
  recordShot((time.ticks_ms()) - startSignalTicks)
  pass
btnB.wasPressed(buttonB_wasPressed)

@timerSch.event('timeRefreshTick')
def ttimeRefreshTick():
  global startSignalTicks, shotTimeListMilli, timeMilli, countdownMilli, shotTimeMilli
  refreshTimer(math.fabs(startSignalTicks - (time.ticks_ms())))
  pass

@timerSch.event('timerCountDown')
def ttimerCountDown():
  global startSignalTicks, shotTimeListMilli, timeMilli, countdownMilli, shotTimeMilli
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  refreshTimer(0)
  speaker.sing(889, 1)
  pass


lcd.clear()
title.show()
logoImage.show()
labelTimer.show()
labelReset.show()
labelStart.show()
labelShots.show()
speaker.setVolume(10)
while True:
  wait_ms(2)
