from m5stack import *
from m5ui import *
from uiflow import *
import machine
import time



ledIsGreen = None




# Describe this function...
def handleTargetHit():
  global ledIsGreen
  rgb.setBrightness(100)
  if ledIsGreen:
    ledIsGreen = False
    rgb.setColorAll(0x33cc00)
  else:
    ledIsGreen = True
    rgb.setColorAll(0xffff00)
  timerSch.run('timerLedOff', 30000, 0x01)
  timerSch.stop('timerSampler')
  # Experimentally CED7000 takes around 35ms to trigger,
  # consider the time is spec'ed between 9/100 and 2/100
  # second between shots, use 50ms beep to ensure it is heard
  dac0.beep(1800, 50, 0)
  # debounce to slightly less than 0.01 second e.g. less than one timer tick
  wait_ms(70)
  timerSch.run('timerSampler', 10, 0x00)


def buttonA_wasPressed():
  global ledIsGreen
  handleTargetHit()
  pass
btnA.wasPressed(buttonA_wasPressed)

@timerSch.event('timerSampler')
def ttimerSampler():
  global ledIsGreen
  # ADC0 connect to a piezoelectric "x'mas card" speaker in parallel to a 1M Ohm resistor. Taking small spike as knock input
  if (adc0.read()) > 20:
    handleTargetHit()
  pass

@timerSch.event('timerLedOff')
def ttimerLedOff():
  global ledIsGreen
  rgb.setBrightness(0)
  pass


adc0 = machine.ADC(33)
adc0.width(machine.ADC.WIDTH_12BIT)
adc0.atten(machine.ADC.ATTN_11DB)
dac0 = machine.DAC(25)
rgb.setBrightness(0)
timerSch.run('timerSampler', 1, 0x00)
while True:
  wait_ms(2)
