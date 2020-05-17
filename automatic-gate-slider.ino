#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  pinMode(pos1, OUTPUT);
  pinMode(neg2, OUTPUT);
  pinMode(pos3, OUTPUT);
  pinMode(neg4, OUTPUT);
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
      forward();
  }
  else if(button_press == 2 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //remote button press for reverse
      reverse();
  }
  else if(for_switch_state == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for forward
      forward();
  }
  else if(rev_switch_state == 1 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for reverse
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