# Arduino Action Air Timer

## Hardware
* NodeMCU V1.0.  
* PCF8574 on 1602 display.  
* Microphone sound sensor for Arduino.  
  * LM393 using electret microphone with VCC, GND, OUT.  
* Arduino active buzzer module.  
  * Replaceable by an 3.3v active buzzer, S8055 transistor and a 1K resistor.  
* Two switches.  

## Aruduino Library Dependencies
* EventManager  
* LiquidCrystal_PCF8574  
* RBD_Timer  
* RBD_Button  
* ArduinoLog  

## Design Notes
* Event driven design around EventManager.  
  * Inter-module communication are often "events".  
    * High priority hardware related events.  
    * Soft events.  
    * Some events are parameterized.  
  * EventManager is a good design safe for pushing events from ISR. Good for  
  future extension using interrupt button input.  
  * However, Arduino `loop()` can still push event events to overflow event  
  queue. Do pay attention to that and see `loopCountDown()` for example.  
* Most modules are in Arduino C style exposing only setup/loopXXX, leaving
everything else static.  
* Round trip latency is measured by connecting start signal to stop plate  
and reading debug output from serial console. Results in consistent 15
milliseconds on NodeMCU V 1.0 (Tensilica Xtensa LX106 @ 80 MHz).  

## Porting To Other Arduino Hardware, Display Etc
* Doable, but not tested on AVR that is weaker computation-wise.  
* Change logging level to LOG_LEVEL_NOTICE and see the rest of the info when you
port or debug.
* Your hardware need to provide 7 output data pins, 1 input data pin.  
* `board.h` contains hardware dependent config and pin assignment, port it to
your hardware design.  
* `display.cpp` supports 1602 LCD on PCF8574, implement your new display by
listening to display events. Better yet #ifdef it and contribute back.  
* Per hardware, measure `ROUND_TRIP_LATENCY` as described above and put
calibration into `board.h`.  
