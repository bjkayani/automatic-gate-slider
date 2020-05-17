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