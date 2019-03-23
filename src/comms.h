#ifndef COMMS_H
#define COMMS_H

#include <SPI.h>
#include <WiFi101.h>

//WiFi connection
enum COMMANDS_LIST : unsigned int{
    INVALID = 0,

    GET_HOMEPAGE = 1,

    /* General Information [2,20] */
    QUERY_LOG = 2,
    GET_BATTERY_VOLTAGE = 3,
    GET_BATTERY_CURRENT = 4,
    STOP_HTTP = 5,

    /* Actuators related [21,40] */
    MOVE_FORWARD = 21,
    MOVE_FORWARD_PERIOD = 22,
    MOVE_BACKWARD = 23,
    MOVE_BACKWARD_PERIOD = 24,
    MOVE_STOP = 25,
    MOVE_FORWARD_ALL = 26,
    MOVE_FORWARD_PERIOD_ALL = 27,
    MOVE_BACKWARD_ALL = 28,
    MOVE_BACKWARD_PERIOD_ALL = 29,
    MOVE_STOP_ALL = 30,
    CHANGE_SPEED = 31,
    CHANGE_SPEED_ALL = 32,
    SET_DIRECTION = 33,

    /* Sensors [41,60] */
    GET_MAGNETIC = 41,
    GET_INFRARED = 42,
    GET_RADIO = 43,
    GET_ACOUSTIC = 44
};

//commandExecuter.cpp
void ExecuteACommand(WiFiClient*,COMMANDS_LIST,char*,unsigned int);

//HTTP server
void setupHTTPServer();
void closeHTTPServer();
const unsigned char MAX_CLIENT_PER_SESSION = 3;
const unsigned long MAX_TIME_CLIENT = 5000000ul; /* 5s in us */

#endif

