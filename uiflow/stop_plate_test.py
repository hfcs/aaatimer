from m5stack import *
from m5ui import *
from uiflow import *
import machine
import time

setScreenColor(0xffffff)


ledIsGreen = None


circle0 = M5Circle(160, 120, 15, 0xffffff, 0x000000)
tiltle = M5Title(title="I am a stop plate", x=3, fgcolor=0xFFFFFF, bgcolor=0x0000FF)



def buttonA_wasPressed():
  global ledIsGreen
  dac0.beep(1800, 1, 0)
  pass
btnA.wasPressed(buttonA_wasPressed)

def buttonB_wasPressed():
  global ledIsGreen
  dac0.beep(1800, 5, 0)
  pass
btnB.wasPressed(buttonB_wasPressed)

def buttonC_wasPressed():
  global ledIsGreen
  dac0.beep(1800, 10, 0)
  pass
btnC.wasPressed(buttonC_wasPressed)

@timerSch.event('timerSampler')
def ttimerSampler():
  global ledIsGreen
  # ADC0 connect to a piezoelectric "x'mas card" speaker in parallel to a 1M Ohm resistor. Taking small spike as knock input
  if (adc0.read()) > 20:
    if ledIsGreen:
      ledIsGreen = False
      circle0.setBgColor(0x33ff33)
    else:
      ledIsGreen = True
      circle0.setBgColor(0xffff33)
    timerSch.run('timerLedOff', 30000, 0x01)
    timerSch.stop('timerSampler')
    dac0.beep(1800, 10, 0)
    # debounce to slightly less than 0.01 second e.g. less than one timer tick
    wait_ms(70)
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
dac0 = machine.DAC(26)
timerSch.run('timerSampler', 1, 0x00)
while True:
  wait_ms(2)
