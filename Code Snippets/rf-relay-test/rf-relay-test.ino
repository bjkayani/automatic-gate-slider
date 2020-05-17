#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int pos1 = 9;
int neg2 = 10;
int pos3 = 11;
int neg4 = 12;
int read_time = -500;
int button_press = 0;


void setup() {
  pinMode(pos1, OUTPUT);
  pinMode(neg2, OUTPUT);
  pinMode(pos3, OUTPUT);
  pinMode(neg4, OUTPUT);
  mySwitch.enableReceive(0);
  Serial.begin(9600);
}

void loop() {

  if(mySwitch.available()) {
    
    if(mySwitch.getReceivedValue() == 9560450){
      read_time = millis();
      button_press = 1;
    }
    if(mySwitch.getReceivedValue() == 9560452){
      read_time=millis();
      button_press = 2;
    }
    mySwitch.resetAvailable();
  }

  if((millis() - read_time)<500){ 
    if(button_press == 1){
      forward();
    }
    else if(button_press == 2){
      reverse();
    }
  }
  else if((millis() - read_time)>500){
    off();
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
