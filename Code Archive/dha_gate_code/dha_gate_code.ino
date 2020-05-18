//This was the code used for a slightly different deployment of this project
//using a mosfet based motor driver. It has ramp up and down for the motor. 
//Leaving this here for when I do formalize this upgrade to the system.

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int m1 = 11;
int m2 = 10;
int left_limit = 6;
int right_limit = 3;
int for_switch = 4;
int rev_switch = 5;
int led = 13;

int button_press = 3;
int state = 0;
int left_limit_state = 0;
int right_limit_state = 0;
int for_switch_state = 0;
int rev_switch_state = 0;
float curr_read = 0;
float volt_read = 0;
int count = 0;
const int total_count = 50;
const float curr_thres = 7;
const float  volt_thres = 10;
int ramp_up = 0;

void setup() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(left_limit, INPUT);
  pinMode(right_limit, INPUT);
  pinMode(for_switch, INPUT);
  pinMode(rev_switch, INPUT);
  mySwitch.enableReceive(0);
  //Serial.begin(9600);
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

  if(mySwitch.available()) {
    
    if(mySwitch.getReceivedValue() == 9499268){
      if(button_press != 2){
        button_press = 1; //open gate
      }
    }
    if(mySwitch.getReceivedValue() == 9499266){
      if(button_press != 1){
        button_press = 2; //close gate
      }
    }
    if(mySwitch.getReceivedValue() == 9499265){
      button_press = 3; //stop
    }
    
    mySwitch.resetAvailable();
    
  }

  if(button_press == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //remote button press for forward
      forward();
      digitalWrite(led, HIGH);
  }
  else if(button_press == 2 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //remote button press for reverse
      reverse();
      digitalWrite(led, HIGH);
  }
  else if(for_switch_state == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for forward
      forward();
      digitalWrite(led, HIGH);
  }
  else if(rev_switch_state == 1 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for reverse
    reverse();
    digitalWrite(led, HIGH);
  }
  else if(button_press == 2 && right_limit_state == 1 && count >= total_count){//gate opening locked scenario
    button_press = 3;
    off();
    delay(200);
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    delay(300);
    off();
  }
  else if(button_press == 3 || left_limit_state == 1 || right_limit_state == 1 || for_switch_state == 0 || rev_switch_state == 0 || count > total_count || volt_read <= volt_thres){//all cases for stop
    button_press = 3;
    off();
    digitalWrite(led, LOW);
  }


}

void forward(){
  if(ramp_up == 0){
  for(int i=0; i<255; i++){
    analogWrite(m1, i);
    digitalWrite(m2, LOW);
    delay(10);
  }
  ramp_up = 1;
  }
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
}

void reverse(){
  if(ramp_up == 0){
  for(int i=0; i<255; i++){
    analogWrite(m2, i);
    digitalWrite(m1, LOW);
    delay(10);
  }
  ramp_up = 1;
  }
  digitalWrite(m2, HIGH);
  digitalWrite(m1, LOW);
}

void off(){
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  ramp_up = 0;
}

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
