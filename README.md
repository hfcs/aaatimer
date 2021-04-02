# Action Air Timer

## Problem Statement
Competitive shooting measures time taken over the course of fire (COF). A stop
plate is recommended for recording last shot attempted on a COF for airsoft as
shots are not loud enough for consistent microphone pickup. Competition grade 
hardware are often expensive and hard to source. A home brew solution can be a
good back up.

## Solution Scope
* Support commercial off the shelf stop plates or PC microphones strapped to a
steel plate(see stop plate section).
* Random 1 to 4 seconds delay of start signal after timer trigger.
  * Required by [IPSC rule 8.3.3](https://www.ipsc.org/pdf/RulesAir.pdf).
-----
### Hardware
#### Timer
* M5Stack ESP32 Basic Core IoT Development Kit (Other model would likely work 
as this is Blockly/MicroPython)
  * Timer listens to stop plate signal on ADC
  ```
  M5Stack ADC --- LM386 module --- mini plug socket --- stop plate
  ``` 
* Using your favorite shooting timer app on mobile phone
  * App with adjustable sensitivty and tune towards dry fire level
  * Build a TRRS plug cable to connect with stop plate and external speaker
  * For phone with USB type C or lightning port, an adptor with TRRS support is 
  required, **not all adaptor have TRRS support for microphone**
```
                               TRS plug --- Powered Speaker
                               LRG
                               |||
                 +-------------+||
                 |+-------------+|
                 ||+-------------+
                 |||
                 LRGM                
Mobile Phone --- TRRS plug (in CITA format for Android and iPhone)
                 LRGM
                   ||
                   |+----------+
                   +----------+|
                              ||
                              MG
                              TS socket -- Stop Plate
```
#### Stop Plate
* Stop Plate using M5Stack Atom Lite
##### Knock Sensor
```
Piezoelectric ------------------------------ DAC0 pin 33 on M5Stack Atom Lite
Speaker               |         
                Resistor 1M Ohm
                      |
Piezoelectirc ------------------------------- GND pin on M5Stack Atom Lite
Speaker GND
```

##### Output To CED700 Timer
```
Mini Plug +   ------------------------------- ADC0 pin 25 on M5Stack Atom Lite
Mini Plug GND ------------------------------- GND pin on M5stack Atom Lite
```
-----
### Software
Micro Python built from UIFLow with Blocky, targeting M5Stack devices
#### Timer (Built on M5Stack ESP32 Basic Core IoT Development Kit)
* Open uiflow/main.m5f from UIFlow
* Put your M5Stack device under proper mode (e.g. Internet mode or USB mode) for 
programming
* Hit the "Play" (Right Arrow) button
* logo.png for customized logo
* Time for last 20 stop plate hit is displayed

#### Stop Plate (Built on M5Stack Atom Lite)
* Open uiflow/stop_plate_atomlite.m5f from UIFlow
* Put your M5Stack device under proper mode (e.g. Internet mode or USB mode) for 
programming
* Hit the "Play" (Right Arrow) button
-----
## Design Notes
* Minimalize global variable, but still unavoidlable in Blockly/MicroPython
  * Globals are accessed by event handlers (e.g. button etc), pretty bad for 
  multithreading but necessary evil the way how Blockly works now. Assuming we 
  don't have this advanced MicroPython enabled yet :) 
* **NOT IMPLEMENTED, A HINT FOR FUTURE EXPANSION** ~~Action camera supporting 
SJCAM API over Wifi (tested on SJ4000 Wifi)
[the great guide](http://sj4000programming.sourceforge.net).~~

## Caveats
* Basic ESP32 turn out having enough juice with following caveats
  * The countdown screen refresh taken much of CPU load, buttons are less 
  responsive there but we don't care during count down
  * Hence the refresh timer is stopped before start signal beeps to disable 
  the frequent refresh of stopwatch, plus matching behavior of most shot 
  timers out there
