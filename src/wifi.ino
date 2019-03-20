#include "wifi_conf.h"
#include "time.h"

int status = WL_IDLE_STATUS;

WiFiServer server(ip_port);

/* Forward declaration */
static void printWiFiStatus();

unsigned int getWIFI_ID(){
  unsigned int res = 0;
  if(analogRead(A4) > 1000) res = 2;
  else if(analogRead(A5) > 1000) res = 1;

  for(int j = 0; j < 2*res; j++){
    digitalWrite(13,true);
    blockMili(100);
    digitalWrite(13,false);
    blockMili(100);
  }
  
  return res;
}

void getIPArray(unsigned int ip, unsigned char (&x)[4]){
	x[0] = ip%256;
	ip = (ip-x[0])/256;
	x[1] = ip%256;
	ip = (ip-x[1])/256;
	x[2] = ip%256;
	ip = (ip-x[2])/256;
	x[3] = ip%256;
}

void shineLED(unsigned int i){
	bool flag = false;
	while(i!=0){
	flag = i % 2;
	i /= 2;
	digitalWrite(13,true);
	blockMili(200);
	digitalWrite(13,false);
	blockMili(200);
	digitalWrite(13,flag);
	blockMili(100);
	digitalWrite(13,false);
	blockMili(100);
	digitalWrite(13,true);
	blockMili(200);
	digitalWrite(13,false);
	blockMili(200);
	}
}

bool setupWiFi() {
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Logln("WiFi shield not present");
      // don't continue:
      return false;
    }

    // attempt to connect to WiFi network:
    restart:
    while (status != WL_CONNECTED) {
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      unsigned int wid = getWIFI_ID();
      status = WiFi.begin(ssid[wid], pass[wid]);
      
      // wait 1 seconds for connection:
      blockMili(1000u);
    }

    unsigned char x[4] = {0,0,0,0}, y[4] = {0,0,0,0};
    getIPArray(WiFi.gatewayIP(),x);
    x[3] = 1;
    while(x[3] != y[3] && x[3] < 255){
      x[3]++;
      WiFi.config(x);
      getIPArray(WiFi.localIP(),y);
    }

    if(x[3] >= 255){
      WiFi.disconnect();
      Logln("DCHP Pool full.");
      goto restart;
    }

    shineLED(x[3]);
    server.begin();
    
    printWiFiStatus();
    return true;
  }


  static void printWiFiStatus() {
    Log("SSID: ");
    Logln(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Log("IP Address: ");
    Logln(String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]) );

    long rssi = WiFi.RSSI();
    Log("signal strength (RSSI):");
    Log(String(rssi));
    Logln(" dBm");
}

static unsigned int sendToClient(WiFiClient* client, const char text[]){
    unsigned int i;
	/* Get the index of null-terminating character */
    for(i = 0; i < MAX_CHAR_ARRAY; i++)
		if(text[i] == 0) break;
	
	/* No character found, do not write to client */
    if(i == MAX_CHAR_ARRAY) return 0;
	
	/* Use overloaded function with proper data length n */
    return sendToClient(client, text, i);
}
static unsigned int sendToClient(WiFiClient* client, const char text[], unsigned int n){
    if(text[n - 1] == 0) { /* Check again to prevent overflow to invalid memory */
		unsigned int i = 0;
		
		/* Send data by chunks if too large */
		for(i = 0; i < n; )
			i += client->write( text+i, (n-i>MAX_CHAR_SENT?MAX_CHAR_SENT:n-i) );
		
		return i;
    }
    return 0; /* text array are not null-terminated */
}
static unsigned int sendToClient(WiFiClient* client, String s){
	return client->write(s.c_str(),s.length());
}
