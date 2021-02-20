from m5stack import *
from m5ui import *
from uiflow import *
import time

setScreenColor(0xffffff)


timeMilli = None
countdownMilli = None
parTimeMilli = None
startSignalTicks = None
setupComplete = None


labelTimer = M5TextBox(80, 125, "0.00", lcd.FONT_DejaVu72, 0x000000, rotate=0)
labelA = M5TextBox(39, 213, "Reset", lcd.FONT_DejaVu18, 0x000000, rotate=0)
labelC = M5TextBox(226, 213, "Start", lcd.FONT_DejaVu18, 0x000000, rotate=0)
logoImage = M5Img(116, 0, "res/logo.png", True)
labelB = M5TextBox(115, 200, "Set Par Time", lcd.FONT_Default, 0x000000, rotate=0)
triangleUp = M5Triangle(67, 189, 36, 218, 96, 218, 0x000000, 0xFFFFFF)
triangleDown = M5Triangle(250, 222, 220, 193, 280, 193, 0x000000, 0xFFFFFF)
labelPar = M5TextBox(99, 73, "Par Time:", lcd.FONT_Default, 0x000000, rotate=0)

import random


# set up screen and variable needed for par time input
def initSetParTimeMode():
  global timeMilli, countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  labelA.hide()
  labelB.show()
  labelC.hide()
  triangleUp.show()
  triangleDown.show()
  parTimeMilli = 0

# Describe this function...
def setParModeHandleUp():
  global timeMilli, countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  parTimeMilli = parTimeMilli + 500
  labelTimer.setText(str("%.2f"%((parTimeMilli / 1000))))

# Describe this function...
def setParModeHandleDown():
  global timeMilli, countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  if parTimeMilli > 0:
    parTimeMilli = parTimeMilli - 500
  labelTimer.setText(str("%.2f"%((parTimeMilli / 1000))))

# set up screen for timer mode
def initTimerMode():
  global timeMilli, countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  triangleUp.hide()
  triangleDown.hide()
  labelB.hide()
  labelA.show()
  labelC.show()
  labelTimer.setText(str("%.2f"%(0)))
  labelPar.setText(str((str(((str('Par Time:') + str(("%.2f"%((parTimeMilli / 1000))))))) + str('s'))))

# Refresh screen with timer reading, parameter in milliseconds
def refreshTimer(timeMilli):
  global countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  labelTimer.setText(str("%.2f"%((timeMilli / 1000))))

# Timer reset logic
def resetCountDown():
  global timeMilli, countdownMilli, parTimeMilli, startSignalTicks, setupComplete
  startSignalTicks = 0
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  refreshTimer(0)

# Timer start logic
def startCountDown(countdownMilli):
  global timeMilli, parTimeMilli, startSignalTicks, setupComplete
  if startSignalTicks == 0:
    startSignalTicks = (time.ticks_ms()) + countdownMilli
    timerSch.run('timerCountDown', countdownMilli, 0x01)
    timerSch.run('timeRefreshTick', 57, 0x00)


def buttonA_wasPressed():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  if setupComplete:
    resetCountDown()
  else:
    setParModeHandleUp()
  pass
btnA.wasPressed(buttonA_wasPressed)

def buttonB_wasPressed():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  if not setupComplete:
    setupComplete = True
  initTimerMode()
  pass
btnB.wasPressed(buttonB_wasPressed)

def buttonC_wasPressed():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  if setupComplete:
    startCountDown(random.randint(1000, 4000))
  else:
    setParModeHandleDown()
  pass
btnC.wasPressed(buttonC_wasPressed)

# Handler for countdown stopwatch refresh
@timerSch.event('timeRefreshTick')
def ttimeRefreshTick():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  refreshTimer(time.ticks_diff(startSignalTicks, time.ticks_ms()))
  pass

# Handler for Par Time
@timerSch.event('timerParTime')
def ttimerParTime():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  timerSch.stop('timerParTime')
  labelTimer.setText(' Par ')
  speaker.tone(3500, 500)
  pass

# Handler for random count down
@timerSch.event('timerCountDown')
def ttimerCountDown():
  global parTimeMilli, startSignalTicks, setupComplete, timeMilli, countdownMilli
  timerSch.stop('timerCountDown')
  timerSch.stop('timeRefreshTick')
  timerSch.run('timerParTime', parTimeMilli, 0x01)
  # As last refresh may not show zero counter, zero it before the start signal
  refreshTimer(0)
  labelTimer.setText('Start')
  speaker.tone(3500, 500)
  pass


lcd.clear()
logoImage.show()
labelTimer.show()
speaker.setVolume(10)
setupComplete = False
initSetParTimeMode()
while True:
  wait_ms(2)
