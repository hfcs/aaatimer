# Arduino Action Air Timer

Competitive shooting measures time taken over the course of fire (COF). A stop
plate is recommended for recording last shot attempted on a COF for airsoft as
shots are not loud enough for consistent microphone pickup.

* ~~Support commercial off the shelf stop plates or PC microphones strapped to a
steel plate.~~ Simulate a stop plate hit by button B hardware
* Random 1 to 4 seconds delay of start signal after timer trigger.
  * Required by [IPSC rule 8.3.3](https://www.ipsc.org/pdf/RulesAir.pdf).
* **NOT YET IMPLEMENTED** ~~Action camera supporting SJCAM API over Wifi(tested on SJ4000 Wifi), starting
recording at start signal. Kudos to
[the great guide](http://sj4000programming.sourceforge.net).~~
  * ~~Wifi status is shown in smiley or X at the end of first row display.~~
  * ~~See [caveats section](#caveats) below for possible issues.~~

## Hardware
* M5Stack ESP32 Basic Core IoT Development Kit (Other model would likely work as this is Blockly/MicroPython)
* Microphone sound sensor for Arduino.  
  * LM393 using electret microphone with VCC, GND, OUT.  
  * Unsolder electret microphone and replace with mini plug socket for COTS
  stop plate interface.
  * Hint for next: build stop plate hardware on Piezoelectric(X'mas card) speaker, stop plate light with 555 timer, cable with Ethernet cable as interface socket for reliable and durable design. 
**NOT YET IMPLEMENTED, for reference with old design for stop plate interface to NodeMCU**
![aaatimer implementation on NodeMCU schematic](hardware/aaatimer_schematic.png)

## Building
* Open uiflow/main.m5f from UIFlow
* Put your M5Stack device under proper mode (e.g. Internet mode or USB mode) for programming
* Hit the "Play" (Right Arrow) button

## Design Notes
* Blockly/MicroPython implementation
* Time for last 20 stop plate hit is displayed
* Minimalize global usage, but still unavoidlable in Blockly/MicroPython
  * Globals are accessed by event handlers (e.g. button etc), pretty bad for multithreading but necessary evil the way how Blockly works now. Assuming we don't have this advanced MicroPython enabled yet :) 

## Customization
* logo.png for customized logo

## Caveats
* Stop plate is similated by button B now, the gap as-is
  * Stop plate hardware design: piezoelectric "X'mas card" speaker hook up to a 1MOhm resistor in parallel, feeding this to M5Stack ADC to detect the knock spike
  * Logic to mimic button B handler, may need hystersis logic to debounce the stop plate signal (hint: use event loop and fast periodic timer to poll)
* Basic ESP32 turn out having enough juice with following caveats
  * The countdown screen refresh taken much of CPU load, buttons are less responsive there but we don't care during count down
  * Hence the refresh timer is stopped before start signal beeps to disable the frequent refresh of stopwatch, plus matching behavior of most shot timers out there

## Ideas
Sensor over long cable to GPIO
```
Sensor Vcc —-------------------------------+3.3v
          |                               |
        capacitor 0.1                   pull up 1k
          |                               |
sensor GND—------------------------GND    |
                                  |       |
                         capacitor 0.1    |
                                  |       |
sensor Output ------------------------------gpio
```
