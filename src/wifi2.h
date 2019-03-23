#ifndef WIFI2_H
#define WIFI2_H

extern struct WiFiServer server;
const unsigned int MAX_CHAR_SENT = 1000u;
const unsigned int MAX_CHAR_ARRAY = 10000u;

bool setupWiFi();
unsigned int sendToClient(WiFiClient*,const char[],unsigned int);
unsigned int sendToClient(WiFiClient*,const char[]);
unsigned int sendToClient(WiFiClient*,String);

#endif
