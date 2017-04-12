# Arduino Action Air Timer

Competitive shooting measures time taken over the course of fire (COF). A stop
plate is recommended for recording last shot attempted on a COF for airsoft as
shots are not loud enough for consistent microphone pickup.

* Support commercial off the shelf stop plates or PC microphones strapped to a
steel plate.
* Random 1 to 4 seconds delay of start signal after timer trigger.
  * Required by [IPSC rule 8.3.3](https://www.ipsc.org/pdf/RulesAir.pdf).
* Action camera supporting SJCAM API over Wifi(tested on SJ4000 Wifi), starting
recording at start signal.
  * Wifi status is shown in smiley or X at the end of first row display.
  * See [Wifi caveats](#Wifi caveats) below for possible issues

## Hardware
* NodeMCU V1.0.  
* PCF8574 on 1602 display.
  * I2C auto detection assume this is the only I2C device attached!!!
* Microphone sound sensor for Arduino.  
  * LM393 using electret microphone with VCC, GND, OUT.  
* Arduino active buzzer module.  
  * Replaceable by an 3.3v active buzzer, S8055 transistor and a 1K resistor.  
* Two switches.  

## Aruduino Library Dependencies
* EventManager  
  * Need to modify for more listeners in EventManager.h, e.g.
  `#define EVENTMANAGER_LISTENER_LIST_SIZE         32`
* LiquidCrystal_PCF8574  
* RBD_Timer  
* RBD_Button  
* ArduinoLog  
* ESPAsyncTCP (https://github.com/me-no-dev/ESPAsyncTCP)

## Design Notes
* Event driven design around EventManager.  
  * Inter-module communication are often "events".  
    * High priority hardware related events.  
    * Soft events.  
    * Some events are parameterized.  
  * EventManager is a good design safe for pushing events from ISR. Good for  
  future extension using interrupt button input.  
  * Display is implemented as display events(commands) so we can add support to
  more display hardware.  
  * However, Arduino `loop()` can still push event events to overflow event  
  queue. Do pay attention to that and see `loopCountDown()` for example.  
* Most modules are in Arduino C style exposing only setup/loopXXX, leaving
everything else static.  
* Fatal error are handled by `Log.fatal()` and then `stopSketchLoop()` which
latter prints error message to display.
* Round trip latency is measured by
  * Connecting start signal to stop plate.
  * Use the same logging level as your production as logging affects timing.
  * `#define MEASURE_ROUND_TRIP_LATENCY` in `board.h`.
  * Run a few start sequences and read debug output from serial console.
  Measured consistent 15 milliseconds on NodeMCU V 1.0 (Tensilica Xtensa LX106 @ 80 MHz).

## Porting To Other Arduino Hardware, Display Etc
* Doable, but not tested on AVR that is weaker than ESP8266 computation-wise.  
* Must have I2C LCD as the only I2C device attached, or you will need to
hardcode your I2C address in `display.cpp`
* Change logging level to LOG_LEVEL_NOTICE and see the rest of the info when you
port or debug.
* Your hardware need to provide 7 output data pins, 1 input data pin.  
* `board.h` contains hardware dependent config and pin assignment, port it to
your hardware design.  
* `display.cpp` supports 1602 LCD on PCF8574, implement your new display by
listening to display events. Better yet #ifdef it and contribute back.  
* Per hardware, measure `ROUND_TRIP_LATENCY` as described above and put
calibration into `board.h`.
* Implement your hardware specific Wifi to interface in `acdtcam.h`.

## Wifi caveats
* On NodeMCU hardware, spurious hit signals are observed when Wifi is
disconnected, inevitably mess up current timing session (recommend range
officer ordering a reshoot for equipment failure). Current workaround shuts
off Wifi once disconnect is detected so we can restart new timing session
without such issue.
* Timer can hang when action camera is unplug right before camera API call
dispatch. For ESP8266 implementation, `AsyncPrinter::connect()` never
returns under said circumstances. Future hardware(backed by RTOS) or network
library may be able to resolve such situation.
