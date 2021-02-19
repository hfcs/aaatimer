from m5stack import *
from m5ui import *
from uiflow import *
import machine
import time

setScreenColor(0xffffff)


ledIsGreen = None


circle0 = M5Circle(160, 120, 15, 0xffffff, 0x000000)
tiltle = M5Title(title="I am a stop plate", x=3, fgcolor=0xFFFFFF, bgcolor=0x0000FF)



@timerSch.event('timerSampler')
def ttimerSampler():
  global ledIsGreen
  if (adc0.read()) < 4000:
    if ledIsGreen:
      ledIsGreen = False
      circle0.setBgColor(0x33ff33)
    else:
      ledIsGreen = True
      circle0.setBgColor(0xffff33)
    timerSch.run('timerLedOff', 10000, 0x01)
    timerSch.stop('timerSampler')
    # debounce to slightly less than 0.01 second e.g. less than one timer tick
    wait_ms(80)
    timerSch.run('timerSampler', 10, 0x00)
  pass

@timerSch.event('timerLedOff')
def ttimerLedOff():
  global ledIsGreen
  circle0.setBgColor(0xffffff)
  pass


adc0 = machine.ADC(36)
adc0.width(machine.ADC.WIDTH_12BIT)
adc0.atten(machine.ADC.ATTN_11DB)
timerSch.run('timerSampler', 1, 0x00)
while True:
  wait_ms(2)
