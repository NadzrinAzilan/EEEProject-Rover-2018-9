#ifndef HTTP_CONTENT_H
#define HTTP_CONTENT_H

char pass_auth[20] = "123";

enum HTTP_HEADER_ENUM : unsigned char{
  INVALID2 = 0,
  HOST = 1,
  CONNECTION = 2,
  AUTH = 3
};

const char HTTP_HEADER_MAX_VAL = 5;
const char HTTP_HEADER_TYPE_LEN[HTTP_HEADER_MAX_VAL] = {6, 12, 12, 8, 6};
const char HTTP_HEADER_TYPE[][30] = {
  "Host: ",
  "Connection: ",
  "User-Agent: ",
  "Accept: ",
  "Auth: "
};

const char HTTP_HEADER_MAX_CHAR = 100; /* Maximum header length allocated */
struct HTTP_REQUEST_LINE{
    char METHOD[4];
    char* ABS_PATH;
    char HTTP_VER[9];
};

const char HTTP_HEADER_CONTENT_SEPERATOR[5] = {13,10,13,10,0}; /* '\r\n\r\n' */

/* HTML data, need to be moved to internal memory flash storage */
const char HTTP_404_RESPONSE[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nConnection: close\r\nCache-Control: no-cache, no-store, must-revalidate\r\n\r\n<!DOCTYPE><html><head></head><body><p>Invalid</p></body></html>";
const char HTTP_STD_RESPONSE[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nCache-Control: no-cache, no-store, must-revalidate\r\n\r\n";
const char HTTP_STD_GET_HOMEPAGE[] = "<!DOCTYPE html><head><meta charset=\"utf-8\"/><title>EEERover Loxopholis</title><style>body{color: white;background-color: black;text-align: center;}.sensorList{text-align: center;margin-left:auto; margin-right:auto;}.sensorList td{width: 15em;}.sensorList td:nth-child(1){text-align: left;}input[type=checkbox]{/* Double-sized Checkboxes */-ms-transform: scale(1.5); /* IE */-moz-transform: scale(1.5); /* FF */-webkit-transform: scale(1.5); /* Safari and Chrome */-o-transform: scale(1.5); /* Opera */transform: scale(1.5);padding: 10px;margin-right: 10px;}#motorSpeed{-webkit-appearance: none;width: 50%;height: 25px;outline: none;}.tabcontent{display: none;padding: 6px 12px;border: 1px solid #ccc;margin-top: 10px;}.btn-group input[type=\"button\"]{background-color: rgb(0, 51, 117);border: 1px solid green;color: white;padding: 10px 24px;cursor: pointer;float: center;}.btn-group button:not(:last-child){border-right: none;}.btn-group:after{content: \"\";clear: both;display: table;}.btn-group button:hover{background-color: #002b7c;}.sectionDiv{font-weight: bold;font-size: 23px;}#joystick_main{align-items: center;display: flex;height: 100px;justify-content: center;margin: auto;width: 100px;border: 5px white solid;}#joystick_wrapper{border: 1px solid black;width: 30px;height: 30px;}.joystick{background-color: blue;border-radius: 100%;cursor: pointer;height: 100%;user-select: none;width: 100%;touch-action: none;}</style><script type=\"text/javascript\">var auth_id=\"auth\";function sendAJAX(resource_URL, callbackFunc){var xhttp=new XMLHttpRequest();xhttp.onreadystatechange=callbackFunc;xhttp.timeout=1000;xhttp.open(\"GET\", resource_URL, true);xhttp.setRequestHeader(\"Cache-Control\",\"no-store, must-revalidate\");xhttp.setRequestHeader(\"Auth\",document.getElementById(auth_id).value);xhttp.send();}const scale=(num, in_min, in_max, out_min, out_max)=>{return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;};function updateCoords(){document.getElementById(x_coord[0]).innerHTML=scale(Math.round(joystick.getPosition().x*scaler),0,200,min_joystick,max_joystick);document.getElementById(y_coord[0]).innerHTML=-1*scale(Math.round(joystick.getPosition().y*scaler),0,200,min_joystick,max_joystick);}function setSendCoords(s){if(s) timer_joy_id=setInterval(updateCoordsToServer, time_interval_send_joystick);else clearInterval(timer_joy_id);}function updateSendCoordsPeriod(x){setSendCoords(false);time_interval_send_joystick=x;setSendCoords(document.getElementById(\"joy_en\").checked);}function updateCoordsToServer(){let el=[null,null];if(use_textjoy){el[0]=document.getElementById(x_coord[1]).value;el[1]=document.getElementById(y_coord[1]).value;}else{el[0]=document.getElementById(x_coord[0]).innerHTML;el[1]=document.getElementById(y_coord[0]).innerHTML;}xc=Math.round(el[0]);yc=Math.round(el[1]);if(prev_val[0] !==xc || prev_val[1] !==yc){prev_val[0]=xc;prev_val[1]=yc;x_neg=(xc<0?'1':'0');y_neg=(yc<0?'1':'0');xc=Math.abs(xc).toString().padStart(3,'0'); yc=Math.abs(yc).toString().padStart(3,'0');requestData('P',x_neg+y_neg+xc+yc);}else{prev_val[0]=xc;prev_val[1]=yc;}}/*Joystick*/function createJoystick(parent, arg_diff){const maxDiff=arg_diff;const stick=document.createElement('div');stick.classList.add('joystick');stick.addEventListener('mousedown', handleMouseDown);document.addEventListener('mousemove', handleMouseMove);document.addEventListener('mouseup', handleMouseUp);stick.addEventListener('touchstart', handleMouseDown);document.addEventListener('touchmove', handleMouseMove);document.addEventListener('touchend', handleMouseUp);let dragStart=null;let currentPos={x: 0, y: 0};function handleMouseDown(event){stick.style.transition='0s';if (event.changedTouches){dragStart={x: event.changedTouches[0].clientX,y: event.changedTouches[0].clientY,};return;}dragStart={x: event.clientX,y: event.clientY,};}function handleMouseMove(event){if (dragStart===null) return;event.preventDefault();if (event.changedTouches){event.clientX=event.changedTouches[0].clientX;event.clientY=event.changedTouches[0].clientY;}const xDiff=event.clientX - dragStart.x;const yDiff=event.clientY - dragStart.y;const angle=Math.atan2(yDiff, xDiff);const distance=Math.min(maxDiff, Math.hypot(xDiff, yDiff));const xNew=distance * Math.cos(angle);const yNew=distance * Math.sin(angle);stick.style.transform=`translate3d(${xNew}px, ${yNew}px, 0px)`;currentPos={x: xNew, y: yNew};}function handleMouseUp(event){if (dragStart===null) return;stick.style.transition='.2s';stick.style.transform=`translate3d(0px, 0px, 0px)`;dragStart=null;currentPos={x: 0, y: 0};}parent.appendChild(stick);return{getPosition: ()=> currentPos};}document.addEventListener('DOMContentLoaded', setup, false);var timer_id,time_interval_sensor=1000,scaler=6.65,min_joystick=0,max_joystick=255, joystick_diff=30;var x_coord=[\"x_coord\", \"x_joy\"], y_coord=[\"y_coord\",\"y_joy\"], timer_joy_id=-1, time_interval_joystick=20,time_interval_send_joystick=1000;var sensor_en={'m':false,'i':false,'r':false,'a':false}, use_textjoy=false, prev_val=[0,0];var sensor_id=[\"sensorMagnetText\",\"sensorInfraredText\",\"sensorRadioText\",\"sensorAcousticText\"];var motor_tab=[\"tc_basic_control\", \"tc_joystick\"];function setup(){document.getElementById(\"motorSpeedTxt\").innerHTML=document.getElementById(\"motorSpeed\").value;timer_id=window.setInterval(getSensorData, time_interval_sensor);setInterval(updateCoords, time_interval_joystick);document.getElementById(\"joy_en\").checked=false;time_interval_send_joystick=document.getElementById(\"joy_period\").value;joystick=createJoystick(document.getElementById('joystick_wrapper'),joystick_diff);}function callbackFunc(x, id, prefix=\"\", suffix=\"\"){x=x.target;if(x.status==200)document.getElementById(id).innerHTML=prefix + x.responseText + suffix;else if(x.status !=0)document.getElementById(id).innerHTML=\"Error: \" + x.status;}function callbackSensor(x, id){x=x.target;if(x.status==200){var tmp=parseInt(x.responseText);if(id==sensor_id[0]){var tmp2=\"No signal\";if(tmp==1) tmp2=\"Downward signal\";else if(tmp==2) tmp2=\"Upward signal\";document.getElementById(id).innerHTML=tmp2;}else if(id==sensor_id[1]){if([0,1,2].indexOf(tmp)==-1) tmp=3;document.getElementById(id).innerHTML=[\"No signal\", \"353Hz\", \"571Hz\", \"Error\"][tmp];}else if(id==sensor_id[2]){var t=[0,0,0,0],tmp2=\"No signal\";t[0]=tmp%256; tmp/=256;t[1]=tmp%256; tmp/=256;t[2]=tmp%256; tmp/=256;t[3]=tmp%256;if(!(t[0]==0 && t[1]==0 && t[2]==0 && t[3]==0)) tmp2=String.fromCharCode.apply(null, t);document.getElementById(id).innerHTML=tmp2;}else if(id==sensor_id[3]){document.getElementById(id).innerHTML=(tmp==1?\"Signal detected\":\"No signal\");}else document.getElementById(id).innerHTML=x.responseText;}else if(x.status !=0)document.getElementById(id).innerHTML=\"Error: \" + x.status;}function getSensorData(){['m','i','r','a'].forEach(function(value,index, array){var tmp=this[index];if(sensor_en[value])sendAJAX(value, (function(x){callbackSensor(x, tmp);}) );}, sensor_id);}function requestData(resource_URL, args=0){if(args!==0) resource_URL+=\"/\"+args;sendAJAX(resource_URL, callbackFunc1);}function callbackFunc1(x){callbackFunc(x, \"callbackText\");}function tab_motor_toggle(id){var tc_current=\"tc_\"+id.substring(3);var tmp=document.getElementById(tc_current).style.display;for (i=0; i < motor_tab.length; i++){if(i==0){document.getElementById(\"motorSpeed\").value=document.getElementById(\"motorSpeedTxt\").innerHTML=0;document.getElementById(\"motorSpeed\").onchange();}if(motor_tab[i]==tc_current) continue;var tabcontent=document.getElementById(motor_tab[i]);tabcontent.style.display=\"none\";}document.getElementById(tc_current).style.display=(tmp==\"none\"||tmp==\"\"?\"block\":\"none\");}</script></head><body><h1>EEERover Loxopholis Remote Page</h1><span>Password: <input id=\"auth\" type=\"password\" length=20 value=\"\"></input></span><p class=\"sectionDiv\">Sensors</p><table class=\"sensorList\"><tr><td><p><input type=\"checkbox\" id=\"radio_en\" onchange=\"sensor_en[this.id[0]]=this.checked;\"/>Radio: <span id=\"sensorRadioText\">No signal</span></p></td><td><p><input type=\"checkbox\" id=\"infrared_en\" onchange=\"sensor_en[this.id[0]]=this.checked;\"/>Infrared: <span id=\"sensorInfraredText\">No signal</span></p></td></tr><tr><td><p><input type=\"checkbox\" id=\"magnetic_en\" onchange=\"sensor_en[this.id[0]]=this.checked;\"/>Magnetic: <span id=\"sensorMagnetText\">No signal</span></p></td><td><p><input type=\"checkbox\" id=\"acoustic_en\" onchange=\"sensor_en[this.id[0]]=this.checked;\"/>Acoustic: <span id=\"sensorAcousticText\">No signal</span></p></td></tr></table><p class=\"sectionDiv\">Message from Arduino:</p><p id=\"callbackText\"><br/></p><p class=\"sectionDiv\">General Information</p><div class=\"btn-group\"><input type=\"button\" id=\"QinputButton\" value=\"Query\" onclick=\"requestData(this.id[0])\"/><input type=\"button\" id=\"VinputButton\" value=\"Voltage\" onclick=\"requestData(this.id[0])\"/><input type=\"button\" id=\"IinputButton\" value=\"Current\" onclick=\"requestData(this.id[0])\"/></div><p class=\"sectionDiv\">Motor Control</p><div class=\"btn-group\"><input type=\"button\" class=\"tablinks\" onclick=\"tab_motor_toggle(this.id);\" id=\"tl_basic_control\" value=\"Basic Control\"/><input type=\"button\" class=\"tablinks\" onclick=\"tab_motor_toggle(this.id);\" id=\"tl_joystick\" value=\"Joystick\"/></div><div id=\"tc_basic_control\" class=\"tabcontent\"><div class=\"btn-group\"><input type=\"button\" id=\"WinputButton\" value=\"Move Forward\" onclick=\"requestData(this.id[0])\"/><input type=\"button\" id=\"winputButton\" value=\"Move Forward for 5 seconds\" onclick=\"requestData(this.id[0],5)\"/><input type=\"button\" id=\"EinputButton\" value=\"Move Backward\" onclick=\"requestData(this.id[0])\"/><input type=\"button\" id=\"einputButton\" value=\"Move Backward for 5 seconds\" onclick=\"requestData(this.id[0],5)\"/><input type=\"button\" id=\"sinputButton\" value=\"Stop Wheel\" onclick=\"requestData(this.id[0])\"/></div><p>Motor Speed</p><div style=\"width=100%\"><input type=\"range\" min=\"0\" max=\"255\" value=\"0\" id=\"motorSpeed\" oninput=\"this.nextElementSibling.firstElementChild.innerHTML=this.value;\"onchange=\"requestData('c',this.value);\"><p>Value: <span id=\"motorSpeedTxt\">50</span></p></div></div><div id=\"tc_joystick\" class=\"tabcontent\"><div id=\"joystick_main\"><div id=\"joystick_wrapper\"></div></div><div>X: <span id=\"x_coord\"></span> Y: <span id=\"y_coord\"></span></div><div><div>Send To Arduino: <input type=\"checkbox\" id=\"joy_en\" onchange=\"setSendCoords(this.checked);\"/></div><div>Enable textbox: <input type=\"checkbox\" id=\"text_joy_en\" onchange=\"use_textjoy=this.checked;\"/></div><div>Period of update: <input type=\"text\" value=1000 id=\"joy_period\" onchange=\"updateSendCoordsPeriod(this.value);\"/></div><div>X: <input type=\"text\" value=0 id=\"x_joy\"/></div><div>Y: <input type=\"text\" value=0 id=\"y_joy\"/></div></div></div></body></html>";


#endif

