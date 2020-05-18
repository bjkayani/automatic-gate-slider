// This was an experiment to estimate gate position based on time elapsed.
// It didnt work for some reasons but leaving this here if I try to implement
// position estimation again. 

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int pos1 = 9;
int neg2 = 10;
int pos3 = 11;
int neg4 = 12;
int left_limit = 6;
int right_limit = 7;
int for_switch = 4;
int rev_switch = 5;

int button_press = 3;
int state = 0;
int left_limit_state = 0;
int right_limit_state = 0;
int for_switch_state = 0;
int rev_switch_state = 0;
float curr_read = 0;
float volt_read = 0;
int count = 0;
const int total_count = 100;
const float curr_thres = 5;
const float  volt_thres = 10;

int open_time_end = 0;
int close_time_end = 0;
int open_time = 0;
int close_time = 0;
long int total_time_interval = 0;
int time_interval = 0;
int gate_pos = 0; //Variable to store position of the gate from 0(Open) to 100(Close)

boolean open_end_flag = false;
boolean close_end_flag = false;
boolean open_flag = false;
boolean close_flag = false;

void setup() {
  pinMode(pos1, OUTPUT);
  pinMode(neg2, OUTPUT);
  pinMode(pos3, OUTPUT);
  pinMode(neg4, OUTPUT);
  pinMode(left_limit, INPUT);
  pinMode(right_limit, INPUT);
  pinMode(for_switch, INPUT);
  pinMode(rev_switch, INPUT);
  mySwitch.enableReceive(0); // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableTransmit(3);
  Serial.begin(9600);
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
    if(mySwitch.getReceivedValue() == 9499268 || mySwitch.getReceivedValue() == 9560452 || mySwitch.getReceivedValue() == 16146052 ||  mySwitch.getReceivedValue() == 16693380){
      if(button_press != 2){
        button_press = 1; //forward - close gate
      }
    }
    if(mySwitch.getReceivedValue() == 9560450 || mySwitch.getReceivedValue() == 9499266 || mySwitch.getReceivedValue() == 16146050 || mySwitch.getReceivedValue() == 16693378){
      if(button_press != 1){
        button_press = 2; //reverse - open gate
      }
    }
    if(mySwitch.getReceivedValue() == 9499265 || mySwitch.getReceivedValue() == 9560449 || mySwitch.getReceivedValue() == 16146049 || mySwitch.getReceivedValue() ==  16693377){
      button_press = 3; //stop
    }
     
    mySwitch.resetAvailable();
    
  }

  if(button_press == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //closing gate - forward
      if(right_limit_state == 1 && close_end_flag == false){ 
        close_time_end = millis();
        close_end_flag = true;
        //Serial.println("Closing gate for end position - time recorded");
      }
      else if(close_flag == false){
        close_time = millis();
        close_flag = true;
        //Serial.println("Closing gate for in between position - time recorded");
      }
      forward();
      //Serial.println("Forward Case - Wireless");
  }
  else if(button_press == 2 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //opening gate - reverse
      if(left_limit_state == 1 && open_end_flag == false){ 
        open_time_end = millis();
        open_end_flag = true;
        //Serial.println("Opening gate for end position - time recorded");
      }
      else if(open_flag == false){
        open_time = millis();
        open_flag = true;
        //Serial.println("Opening gate for in between position - time recorded");
      }
      reverse();
      //Serial.println("Reverse Case - Wireless");
  }
  else if(for_switch_state == 1 && left_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for forward
      forward();
      //Serial.println("Forward Case - Manual");
  }
  else if(rev_switch_state == 1 && right_limit_state == 0 && count < total_count && volt_read >= volt_thres){ //wired button press for reverse
      reverse();
      //Serial.println("Reverse Case - Manual");
  }
  else if(button_press == 2 && left_limit_state == 1 && count >= total_count){//gate opening locked scenario
    Serial.println("Locked Case");
    button_press = 3;
    off();
    delay(200);
    forward();
    delay(100);
    off();
  }
  else if(button_press == 3 || left_limit_state == 1 || right_limit_state == 1 || for_switch_state == 0 || rev_switch_state == 0 || count >= total_count || volt_read <= volt_thres){//all cases for stop
    if(close_end_flag == true && left_limit_state == 1){
      Serial.print("close stop ");
      Serial.println(millis());
      total_time_interval = millis() - close_time_end;
      total_time_interval = abs(total_time_interval);
      Serial.print("End to End time recorded as: ");
      Serial.println(total_time_interval);
      open_end_flag = false;
      close_end_flag = false;
      open_flag = false;
      close_flag = false;
    }
    else if(close_flag == true){
      time_interval = millis() - close_time;
      time_interval = abs(time_interval);
      Serial.println("Motion time recorded as: ");
      Serial.println(time_interval);
      gate_pos = gate_pos+((100*time_interval)/21000); //update the new gate position based on timing
      Serial.print("Gate Position: ");
      Serial.println(gate_pos);
      
      open_end_flag = false;
      close_end_flag = false;
      open_flag = false;
      close_flag = false;
    }
    else if(open_end_flag == true && right_limit_state == 1){
      Serial.print("open stop ");
      Serial.println(millis());
      total_time_interval = millis() - open_time_end;
      total_time_interval = abs(total_time_interval);
      Serial.print("End to End time recorded as: ");
      Serial.println(total_time_interval);
      
      open_end_flag = false;
      close_end_flag = false;
      open_flag = false;
      close_flag = false;
    }
    else if(open_flag == true){
      time_interval = millis() - open_time;
      time_interval = abs(time_interval);
      Serial.println("Motion time recorded as: ");
      Serial.println(time_interval);
      gate_pos = gate_pos-((100*time_interval)/21000); //update new gate position based on timing
      Serial.print("Gate Position: ");
      Serial.println(gate_pos);
      
      open_end_flag = false;
      close_end_flag = false;
      open_flag = false;
      close_flag = false;
    }
    //Serial.println("All Stop Case ");
    button_press = 3;
    off();
  }
  if(left_limit_state == 1){
    gate_pos = 100;
  }
  else if(right_limit_state == 1){
    gate_pos = 0;
  }
  
}

void forward(){
  digitalWrite(pos1, HIGH);
  digitalWrite(pos3, LOW);
  digitalWrite(neg2, HIGH);
  digitalWrite(neg4, LOW);
}

void reverse(){
  digitalWrite(pos3, HIGH);
  digitalWrite(pos1, LOW);
  digitalWrite(neg4, HIGH);
  digitalWrite(neg2, LOW);
}

void off(){
  digitalWrite(pos3, LOW);
  digitalWrite(pos1, LOW);
  digitalWrite(neg4, LOW);
  digitalWrite(neg2, LOW);
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
