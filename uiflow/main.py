from m5stack import *
from m5ui import *
from uiflow import *

setScreenColor(0xffffff)





image0 = M5Img(120, 71, "res/logo.png", True)
labelTimer = M5TextBox(36, 210, "Timer", lcd.FONT_DejaVu18, 0x000000, rotate=0)
labelPar = M5TextBox(202, 210, "Par Time", lcd.FONT_DejaVu18, 0x000000, rotate=0)

def buttonA_wasPressed():
  # global params
  execfile("apps/timer.py")
  pass
btnA.wasPressed(buttonA_wasPressed)

def buttonC_wasPressed():
  # global params
  execfile("apps/par_time.py")
  pass
btnC.wasPressed(buttonC_wasPressed)


labelTimer.show()
labelPar.show()
while True:
  wait_ms(2)