#include "debug.h"
#include "actuators.h"
#include "sensors.h"
#include "HTTP_content.h"

void ExecuteACommand(WiFiClient* client, COMMANDS_LIST c, char* args, unsigned int args_n){
    String tmp = HTTP_STD_RESPONSE;
    switch (c)
    {
        //General Information
        case COMMANDS_LIST::GET_HOMEPAGE:
            sendToClient(client, HTTP_STD_GET_HOMEPAGE, sizeof HTTP_STD_GET_HOMEPAGE);
            break;
        case COMMANDS_LIST::QUERY_LOG:
            tmp += String(_log_diag);
            sendToClient(client, tmp);
            resetLOG();
            break;
        case COMMANDS_LIST::GET_BATTERY_VOLTAGE:
            tmp += "voltage";
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::GET_BATTERY_CURRENT:
            tmp += "current";
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::STOP_HTTP:
            closeHTTPServer();
            break;

        //Actuators
        case COMMANDS_LIST::MOVE_FORWARD:
            wheel_moveForward(static_cast<WHEEL_SIDE>(args[0]==1));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_FORWARD_PERIOD:
            wheel_moveForwardWithPeriod(static_cast<WHEEL_SIDE>(args[0]==1), atoi(args)+1);
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_BACKWARD:
            wheel_moveBackward(static_cast<WHEEL_SIDE>(args[0]==1));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_BACKWARD_PERIOD:
            wheel_moveBackwardWithPeriod(static_cast<WHEEL_SIDE>(args[0]==1), atoi(args+1));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_STOP:
            wheel_stop(static_cast<WHEEL_SIDE>(args[0]==1));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_FORWARD_ALL:
            wheel_moveForward(WHEEL_SIDE::LEFT);
            wheel_moveForward(WHEEL_SIDE::RIGHT);
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_FORWARD_PERIOD_ALL:
            wheel_moveForwardWithPeriod(WHEEL_SIDE::LEFT, atoi(args));
            wheel_moveForwardWithPeriod(WHEEL_SIDE::RIGHT, atoi(args));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_BACKWARD_ALL:
            wheel_moveBackward(WHEEL_SIDE::LEFT);
            wheel_moveBackward(WHEEL_SIDE::RIGHT);
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_BACKWARD_PERIOD_ALL:
            wheel_moveBackwardWithPeriod(WHEEL_SIDE::LEFT, atoi(args));
            wheel_moveBackwardWithPeriod(WHEEL_SIDE::RIGHT, atoi(args));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::MOVE_STOP_ALL:
            wheel_stop(true, true);
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::CHANGE_SPEED_ALL:
            wheel_change_speed((unsigned int)atoi(args), (unsigned int)atoi(args));
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::SET_DIRECTION:
            args[8] = 0;
            wheel_joystick(atol(args));
            sendToClient(client, tmp);
            break;

        //Sensors
        case COMMANDS_LIST::GET_MAGNETIC:
            tmp += (String)(readMagneticSensors());
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::GET_INFRARED:
            tmp += (String)(readInfraredSensors());
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::GET_RADIO:
            tmp += (String)(readRadioSensors());
            sendToClient(client, tmp);
            break;
        case COMMANDS_LIST::GET_ACOUSTIC:
            tmp += (String)(readAcousticSensors());
            sendToClient(client, tmp);
            break;
			
        /* Return 404 page for invalid code */
        case COMMANDS_LIST::INVALID:
            sendToClient(client, HTTP_404_RESPONSE, sizeof HTTP_404_RESPONSE);
            break;
    		default:
      			sendToClient(client, HTTP_404_RESPONSE, sizeof HTTP_404_RESPONSE);
      			break;
    }
}

