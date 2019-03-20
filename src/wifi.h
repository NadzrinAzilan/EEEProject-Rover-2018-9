#ifndef WIFI_H
#define WIFI_H

bool setupWiFi();
extern struct WiFiServer server;
static unsigned int sendToClient(WiFiClient*,const char[],unsigned int);
static unsigned int sendToClient(WiFiClient*,const char[]);
static unsigned int sendToClient(WiFiClient*,String);
const unsigned int MAX_CHAR_SENT = 1000u;

#endif