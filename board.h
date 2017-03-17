// NodeMCU hardware definition

#define ADC_PIN A0

#define DISPLAY_I2C_SCL_PIN D1
#define DISPLAY_I2C_SDA_PIN D2
#define START_PIN D3
#define RESET_PIN D4
#define STOP_PLATE_PIN D5
#define LED_1_PIN D6
#define LED_2_PIN D7
#define BUZZER_PIN D8

// Measured by connecting buzzer pin to stop plate pin, a consistent latency of
// the whole stopwatch system end to end for all timing sessions
// On NodeMCU 1.0 (ESP12E), it was experienmentally measured as 15ms
#define ROUND_TRIP_LATENCY (15)
