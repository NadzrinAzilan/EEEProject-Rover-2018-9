typedef const unsigned int(*T_READ)();
typedef void(*T_WRITE)(unsigned int);
const unsigned int input_pin = A0;
const char input_pin_str[2] = {'A', '0'};
const unsigned int output_pin = 2;
const T_READ input = []()->const unsigned int{return analogRead(input_pin);};
const T_WRITE output = [](unsigned int value)->void{analogWrite(output_pin, value);};
const unsigned int D_PIN[] = {3,4,5,6,7,8,9,10,11,12};
const unsigned int A_PIN[] = {A1,A2,A3,A4,A5};
const unsigned int DIG_SIZE = sizeof(D_PIN)/sizeof(D_PIN[0]);
const unsigned int ANA_SIZE = sizeof(A_PIN)/sizeof(A_PIN[0]);

void set_pin_output(unsigned int p){
    pinMode(p, OUTPUT);
}
void set_pin_input(unsigned int p){
    pinMode(p, INPUT);
}

bool test_input_pin(){
    out(String("Connect 3.3V to pin ") + String(input_pin_str) + String("...\n"));
    get_response();
    bool tmp = input()>1000;
    out(String("Connect GND to pin ") + String(input_pin_str) + String("...\n"));
    get_response();
    return tmp & (input()<30);
}
bool test_output_pin(){
    out(String("Connect pin ") + String(output_pin) + String(" to pin ") + String(input_pin_str) + String("...\n"));
    output(255);
    bool tmp = input()>1000;
    output(0);
    return tmp & (input()<30);
}

template<class T>
void out(T x){
    Serial.print(x);
}
void get_response(){
    while(!Serial.available()) ;
    Serial.read();
}

void write_summary(bool* dig, bool* pwm, bool* dig_in, bool* ana, bool* ana_in){
    out("\nFailure detected: ");

    for(unsigned int i = 0; i < DIG_SIZE; i++){
        out(String("Digital pin ") + String(D_PIN[i]) + String(": "));
        if(!dig[i]) out("output ");
        if(!pwm[i]) out("pwm ");
        if(!dig_in[i]) out("input ");
        out("\n");
    }

    for(unsigned int i = 0; i < ANA_SIZE; i++){
        out(String("Analog pin A") + String(A_PIN[i]) + String(": "));
        if(!ana[i]) out("output ");
        if(!ana_in[i]) out("input ");
        out("\n");
    }
}

void end_func(){
    delay(1000);
    Serial.end();
    while(true);
}

void setup(){
    delay(1000);
    Serial.begin(9600);
    while(!Serial) delay(10);

    set_pin_input(input_pin);
    set_pin_output(output_pin);
    if(test_input_pin()){
        out("Dedicated input pin not working!");
        end_func();
    }
    if(test_output_pin()){
        out("Dedicated output pin not working!");
        end_func();
    }
}

void loop(){
    bool dig_res[DIG_SIZE] = {0};
    bool pwm_res[DIG_SIZE] = {0};
    bool dig_in_res[DIG_SIZE] = {0};
    bool ana_res[DIG_SIZE] = {0};
    bool ana_in_res[DIG_SIZE] = {0};

    //test_digital
    out("Testing output digital capability...\n");
    for(unsigned int i = 0; i < DIG_SIZE; i++){
        set_pin_input(D_PIN[i]);
        out(String("Connect pin ") + String(input_pin_str) + String("to pin ") + String(D_PIN[i]) + String(": \n"));
        get_response();
        set_pin_output(D_PIN[i]);
        digitalWrite(D_PIN[i], HIGH);
        dig_res[i] = input() > 1000;
        digitalWrite(D_PIN[i], LOW);
        dig_res[i] &= input() < 30;
        set_pin_input(D_PIN[i]);
        out(dig_res[i]?"Success\n":"Fail\n");
    }

    //test_PWM
    out("Testing output PWM capability...\n");
    for(unsigned int i = 0; i < DIG_SIZE; i++){
        set_pin_input(D_PIN[i]);
        out(String("Connect pin ") + String(input_pin_str) + String("to pin ") + String(D_PIN[i]) + String(": \n"));
        get_response();
        set_pin_output(D_PIN[i]);
        analogWrite(D_PIN[i], 128);
        pwm_res[i] = pulseIn(input_pin, true, 3000) > 100;
        pwm_res[i] &= pulseIn(input_pin, false, 3000) > 100;
        set_pin_input(D_PIN[i]);
        out(pwm_res[i]?"Success\n":"Fail\n");
    }

    //test_dig_input
    out("Testing input digital capability...\n");
    for(unsigned int i = 0; i < DIG_SIZE; i++){
        set_pin_input(D_PIN[i]);
        out(String("Connect pin ") + String(output_pin) + String("to pin ") + String(D_PIN[i]) + String(": \n"));
        get_response();
        output(255);
        dig_in_res[i] = digitalRead(D_PIN[i]);
        output(0);
        dig_in_res[i] &= !digitalRead(D_PIN[i]);
        set_pin_input(D_PIN[i]);
        out(dig_in_res[i]?"Success\n":"Fail\n");
    }

    //test_analog_output
    out("Testing output analog capability...\n");
    for(unsigned int i = 0; i < ANA_SIZE; i++){
        set_pin_input(A_PIN[i]);
        out(String("Connect pin ") + String(input_pin_str) + String("to pin A") + String(A_PIN[i]) + String(": \n"));
        get_response();
        set_pin_output(A_PIN[i]);
        analogWrite(A_PIN[i], 128);
        ana_res[i] = pulseIn(input_pin, true, 3000) > 100;
        ana_res[i] &= pulseIn(input_pin, false, 3000) > 100;
        set_pin_input(A_PIN[i]);
        out(ana_res[i]?"Success\n":"Fail\n");
    }

    //test_dig_input
    out("Testing input analog capability...\n");
    for(unsigned int i = 0; i < ANA_SIZE; i++){
        set_pin_input(A_PIN[i]);
        out(String("Connect pin ") + String(output_pin) + String("to pin A") + String(A_PIN[i]) + String(": \n"));
        get_response();
        output(255);
        ana_in_res[i] = analogRead(A_PIN[i]) > 1000;
        output(0);
        ana_in_res[i] &= analogRead(A_PIN[i]) < 30;
        set_pin_input(A_PIN[i]);
        out(ana_in_res[i]?"Success\n":"Fail\n");
    }

    write_summary(&(dig_res[0]), &(pwm_res[0]), &(dig_in_res[0]), &(ana_res[0]), &(ana_in_res[0]));

    end_func();
}