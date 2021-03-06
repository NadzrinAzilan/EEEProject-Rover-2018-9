#ifndef PIN_H
#define PIN_H

#define HL(x) x?HIGH:LOW

const unsigned char MOTOR_2_PWM_PIN = 3; /* left motor */
const unsigned char MOTOR_2_DIR_PIN = 2;
const unsigned char MOTOR_1_PWM_PIN = 9; /* right motor */
const unsigned char MOTOR_1_DIR_PIN = 8;
const unsigned char SENSOR_MOTOR_PWM_PIN = 6; /* sensor movement */
const unsigned char SENSOR_MOTOR_DIR_PIN = 6;

const unsigned char RADIO_FREQ_SEL_PIN = 6;

const unsigned char SENSOR_MAGNETIC_PIN_UP = A0;
const unsigned char SENSOR_MAGNETIC_PIN_DOWN = A1;
const unsigned char SENSOR_RADIO_PIN = 0;
const unsigned char SENSOR_ACOUSTIC_PIN = A2;
const unsigned char SENSOR_INFRARED_PIN = A3;

const unsigned char WIFI_SELECT1_PIN = A4;
const unsigned char WIFI_SELECT2_PIN = A5;

const unsigned char LED_PIN = LED_BUILTIN;

//do not use these pins
const unsigned char WIFI_HANDSHAKE = 7;
const unsigned char WIFI_MOSI = 11;
const unsigned char WIFI_MISO = 12;
const unsigned char WIFI_CLK = 13;
const unsigned char WIFI_SD_SS = 4;
const unsigned char WIFI_SS = 10;
const unsigned char WIFI_RST = 5;
const unsigned char UART_RX = 0;
const unsigned char UART_TX = 1;

#endif
