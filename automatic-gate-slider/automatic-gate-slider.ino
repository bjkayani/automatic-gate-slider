 #include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch(); 

// Input Output Definitions

// relay pins
int pos1 = 8;
int neg2 = 9;
int pos3 = 10;
int neg4 = 11;

// pwm pins
//int for_pin = 10;
//int rev_pin = 11;

// limit switches
int left_limit = 3;
int right_limit = 6;

// manual control switches
int for_switch = 4;
int rev_switch = 5;

// Variable Definitions

// State
int button_press = 3;
int state = 0;

// Switch states
int left_limit_state = 0;
int right_limit_state = 0;
int for_switch_state = 0;
int rev_switch_state = 0;

// Voltage and Current
float curr_read = 0;
float volt_read = 0;
int count = 0;
const int total_count = 100;
const float curr_thres = 5;
const float  volt_thres = 10;

void setup() {
  pinMode(pos1, OUTPUT);
  pinMode(neg2, OUTPUT);
  pinMode(pos3, OUTPUT);
  pinMode(neg4, OUTPUT);
//  pinMode(for_pin, OUTPUT);
//  pinMode(rev_pin, OUTPUT);
  pinMode(left_limit, INPUT);
  pinMode(right_limit, INPUT);
  pinMode(for_switch, INPUT);
  pinMode(rev_switch, INPUT);
  mySwitch.enableReceive(0);
  Serial.begin(9600);
  Serial.println("Automatic Gate Controller");
}

void loop() {
  left_limit_state = digitalRead(left_limit);
  right_limit_state = digitalRead(right_limit);  
  for_switch_state = digitalRead(for_switch);
  rev_switch_state = digitalRead(rev_switch);
  
  curr_read = curr_mes();
  volt_read = volt_mes();

  if(curr_read > curr_thres){
    count = count + 1;
  }
  else if(curr_read < curr_thres){
    count = 0;
  }

  // get RC value and change button state accordingly 
  if(mySwitch.available()) {
    if(mySwitch.getReceivedValue() == 9499268 || mySwitch.getReceivedValue() == 9560452 || mySwitch.getReceivedValue() == 16146052 ||  mySwitch.getReceivedValue() == 16693380){
      if(button_press != 2){
        button_press = 1; //open gate
      }
    }
    if(mySwitch.getReceivedValue() == 9560450 || mySwitch.getReceivedValue() == 9499266 || mySwitch.getReceivedValue() == 16146050 || mySwitch.getReceivedValue() == 16693378){
      if(button_press != 1){
        button_press = 2; //close gate
      }
    }
    if(mySwitch.getReceivedValue() == 9499265 || mySwitch.getReceivedValue() == 9560449 || mySwitch.getReceivedValue() == 16146049 || mySwitch.getReceivedValue() ==  16693377){
      button_press = 3; //stop
    }
    mySwitch.resetAvailable(); 
  }

  // State machine
  if(button_press == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //remote button press for forward
      Serial.println("RF Forward");
      forward();
  }
  else if(button_press == 2 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //remote button press for reverse
      Serial.println("RF Reverse");
      reverse();
  }
  else if(for_switch_state == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for forward
      Serial.println("Switch Forward");
      forward();
  }
  else if(rev_switch_state == 1 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for reverse
    Serial.println("Switch Reverse");
    reverse();
  }
  else if(button_press == 2 && left_limit_state == 1 && count >= total_count){ //gate opening locked scenario
    button_press = 3;
    off();
    delay(200);
    forward();
    delay(100);
    off();
  }
  else if(button_press == 3 || left_limit_state == 1 || right_limit_state == 1 || for_switch_state == 0 || rev_switch_state == 0 || count > total_count || volt_read <= volt_thres){//all cases for stop
    button_press = 3;
    off();
  }
}


// functions to control gate motor

void forward(){
  digitalWrite(pos1, HIGH);
  digitalWrite(pos3, LOW);
  digitalWrite(neg2, HIGH);
  digitalWrite(neg4, LOW);
//  digitalWrite(for_pin, HIGH);
//  digitalWrite(rev_pin, LOW);
}

void reverse(){
  digitalWrite(pos3, HIGH);
  digitalWrite(pos1, LOW);
  digitalWrite(neg4, HIGH);
  digitalWrite(neg2, LOW);
//  digitalWrite(for_pin, LOW);
//  digitalWrite(rev_pin, HIGH);
}

void off(){
  digitalWrite(pos3, LOW);
  digitalWrite(pos1, LOW);
  digitalWrite(neg4, LOW);
  digitalWrite(neg2, LOW); 
//  digitalWrite(for_pin, LOW);
//  digitalWrite(rev_pin, LOW);
}

// functions to measure current and voltage

float curr_mes(){
  float average = 0;
  for(int i = 0; i < 10; i++) {
    average = average + abs(analogRead(A0)-512)*0.0722;
    delay(1);
  }
  average = average / 10;
  return average;
}

float volt_mes(){
  float average = 0;
  for(int i = 0; i < 10; i++){
    average = average + (0.028*analogRead(A1));
    delay(1);
  }
  average = average / 10;
  return average;
}
