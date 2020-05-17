// functions to control gate motor

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