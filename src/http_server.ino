#include "comms.h"
#include "time.h"
#include "HTTP_content.h"
#include "wifi2.h"

unsigned char timer_id = 0;
COMMANDS_LIST mapToCommand[255] = {COMMANDS_LIST::INVALID};

/* Forward declaration */
unsigned int parseHTTPRequest(String,COMMANDS_LIST*,char*,unsigned int);
void checkForIncomingRequest();

void setupHTTPServer(){

    /* General information */
    mapToCommand[0] = COMMANDS_LIST::GET_HOMEPAGE;
    mapToCommand['I'] = COMMANDS_LIST::GET_HOMEPAGE;
    mapToCommand['Q'] = COMMANDS_LIST::QUERY_LOG;
    mapToCommand['V'] = COMMANDS_LIST::GET_BATTERY_VOLTAGE;
    mapToCommand['I'] = COMMANDS_LIST::GET_BATTERY_CURRENT;
    mapToCommand['X'] = COMMANDS_LIST::STOP_HTTP;

    /* Actuators Command Code */
    mapToCommand['F'] = COMMANDS_LIST::MOVE_FORWARD;
    mapToCommand['f'] = COMMANDS_LIST::MOVE_FORWARD_PERIOD;
    mapToCommand['B'] = COMMANDS_LIST::MOVE_BACKWARD;
    mapToCommand['b'] = COMMANDS_LIST::MOVE_BACKWARD_PERIOD;
    mapToCommand['S'] = COMMANDS_LIST::MOVE_STOP;
    mapToCommand['W'] = COMMANDS_LIST::MOVE_FORWARD_ALL;
    mapToCommand['w'] = COMMANDS_LIST::MOVE_FORWARD_PERIOD_ALL;
    mapToCommand['E'] = COMMANDS_LIST::MOVE_BACKWARD_ALL;
    mapToCommand['e'] = COMMANDS_LIST::MOVE_BACKWARD_PERIOD_ALL;
    mapToCommand['s'] = COMMANDS_LIST::MOVE_STOP_ALL;
    mapToCommand['c'] = COMMANDS_LIST::CHANGE_SPEED_ALL;
    mapToCommand['P'] = COMMANDS_LIST::SET_DIRECTION;
    mapToCommand['R'] = COMMANDS_LIST::SENSOR_M_CW;
    mapToCommand['C'] = COMMANDS_LIST::SENSOR_M_CCW;

    /* Sensors Command Code */
    mapToCommand['m'] = COMMANDS_LIST::GET_MAGNETIC;
    mapToCommand['i'] = COMMANDS_LIST::GET_INFRARED;
    mapToCommand['r'] = COMMANDS_LIST::GET_RADIO;
    mapToCommand['a'] = COMMANDS_LIST::GET_ACOUSTIC;

	/* Start listening for connections for every 50ms */
    timer_id = registerTimerWithCallback(50000ul, checkForIncomingRequest, true);
}

void closeHTTPServer(){
    unregisterTimer(timer_id, false);
    timer_id = 0;
}


void checkForIncomingRequest() {
    unsigned char nClients = 0;
    WiFiClient client;
    while ((client = server.available())?true:false && (nClients++ < MAX_CLIENT_PER_SESSION) ) {
		/* set timeout for client */
        client.setTimeout(MAX_TIME_CLIENT/1000);
        String currentLine =  "";
        bool currentLineIsBlank = true;
        char cc[sizeof HTTP_HEADER_CONTENT_SEPERATOR] = {0}, auth[sizeof pass_auth] = {0}, args[HTTP_HEADER_MAX_CHAR] = {0};
        unsigned int tmp = 0;
		    COMMANDS_LIST c;
		
        unsigned long time_client = micros();
        while (client.connected() && micros() - time_client < MAX_TIME_CLIENT) {
            if (client.available()) {
				for(tmp = 0; tmp < sizeof cc - 2; tmp++) cc[tmp] = cc[tmp+1];
                cc[(sizeof cc) - 2] = client.read();

                if(strncmp(cc, HTTP_HEADER_CONTENT_SEPERATOR, sizeof HTTP_HEADER_CONTENT_SEPERATOR) != 0)
					currentLine += cc[(sizeof cc) - 2]; /* letter read is not newline char, thus add to currentLine */
                else {
					tmp = parseHTTPRequest(currentLine, &c, &(args[0]), sizeof args, &(auth[0]), sizeof auth);
          
					if(strncmp(pass_auth, auth, sizeof(pass_auth)-1) == 0 || c == COMMANDS_LIST::GET_HOMEPAGE || c == COMMANDS_LIST::INVALID)
						ExecuteACommand(&client, c, &(args[0]), tmp);
					else
						ExecuteACommand(&client, COMMANDS_LIST::INVALID, NULL, 0);
					
					client.flush();
					break;
				}
            }
        }
		
        // give the client time to receive the data
        blockMili(10);

        // close the connection:
        client.stop();
    }
}


unsigned int parseHTTPRequest(String t, COMMANDS_LIST* c, char *args, unsigned int args_n, char *auth, unsigned int auth_n){
    unsigned int i,j;
    HTTP_REQUEST_LINE currentRequest = {0};
    String currentLine;
    bool firstLine,blankLine;
    char header[HTTP_HEADER_MAX_VAL][HTTP_HEADER_MAX_CHAR] = {0};
    size_t size;
    
    //Parse header request
    for(i = 0, currentLine = "", firstLine = blankLine = true; i < t.length() - 1; i++){
        if(t[i] == '\r' && t[i+1] == '\n'){
            i++; /* Skip '\n' from being added to currentLine */
            if(blankLine) break; /* Process header request finished. Data after this is the POST data. */
            else{
                if(firstLine){
                    firstLine = false;
                    strncpy(currentRequest.METHOD, currentLine.substring(0,3).c_str(), (sizeof currentRequest.METHOD) - 1);
                    size = currentLine.indexOf('\x20',4); /* might return -1 if not found */
					if(size > 4){
						size -= 4;
						currentRequest.ABS_PATH = new char[size+1];
						currentRequest.ABS_PATH[size] = 0; /* null terminating character */
						strncpy(currentRequest.ABS_PATH,currentLine.substring(4,size+4).c_str(),size); //Get anything between 4 to [before last 8]
						strncpy(currentRequest.HTTP_VER, currentLine.substring(size+5,size+5+(sizeof currentRequest.HTTP_VER)-1 ).c_str(), 
							(sizeof currentRequest.HTTP_VER)-1 ); //Get last 8 chars
					}
                }
                else {
                    for(unsigned char j = 0; j < HTTP_HEADER_MAX_VAL; j++){
                        if(strncmp(currentLine.c_str(), HTTP_HEADER_TYPE[j], HTTP_HEADER_TYPE_LEN[j]) == 0)
							strncpy(header[j], currentLine.c_str() + HTTP_HEADER_TYPE_LEN[j], 
								min(currentLine.length(), HTTP_HEADER_MAX_CHAR - 1) );
                    }
                }      
            }
            blankLine = true;
            currentLine = "";
        }
        else {
            currentLine += t[i];
            blankLine = false;
        }
    }
    
    strncpy(auth, header[4], min(auth_n,HTTP_HEADER_MAX_CHAR)); /* Copy the password */
    *c = mapToCommand[currentRequest.ABS_PATH[1]];              /* Get the command enum code */
    size = min(args_n-1,size-3);                                /* Get the argument length which must be less than args_n (prevent overflow) */
	                                                            /* -3 because '/x/' is 3 words */
    strncpy(args, currentRequest.ABS_PATH+3, size);             /* And copy arguments to array */
    args[size] = 0;                                             /* Add null-terminator word at the end of array */
    delete[] currentRequest.ABS_PATH;                           /* Delete all allocation using new[] */
    return size+1;                                              /* Return size of argument */
}

