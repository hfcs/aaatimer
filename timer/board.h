// NodeMCU hardware definition

#define ADC_PIN A0

#define DISPLAY_I2C_SCL_PIN D1
#define DISPLAY_I2C_SDA_PIN D2
#define START_PIN D3
#define REVIEW_PIN D4
#define STOP_PLATE_PIN D5
#define BUZZER_PIN D6
#define LED_1_PIN D7
#define LED_2_PIN D8

// A consistent latency of the whole stopwatch system end to end for all timing
// sessions.
// On NodeMCU 1.0 (ESP12E), it was experienmentally measured as < 1ms
// To measure on your hardware
// 1. Connect buzzer pin to stop plate pin
// 2. Use the same logging level as your production as logging affects timing
// 3. #define MEASURE_ROUND_TRIP_LATENCY below
// 4. Run timer and read the first hit from log
// 5. Adjust ROUND_TRIP_LATENCY reported

//#define MEASURE_ROUND_TRIP_LATENCY
#define ROUND_TRIP_LATENCY (0)

#define TIMER_LOG_LEVEL (LOG_LEVEL_WARNING)

#define DISPLAY_CONFIG DISPLAY_PCF8574_LCD1602
