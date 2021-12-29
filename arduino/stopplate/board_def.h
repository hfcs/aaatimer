// Board definition to abstract out board specfic definitions among various Arduino hardware

#if defined(ARDUINO_RASPBERRY_PI_PICO)

#else
#error "unsupported hardware"
#endif