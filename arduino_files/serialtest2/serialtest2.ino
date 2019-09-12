int FR_pin_num = 6;
int FL_pin_num = 9;
int RL_pin_num = 5;
int RR_pin_num = 3; 
 void setup() {
pinMode(FR_pin_num,OUTPUT);
  pinMode(FL_pin_num,OUTPUT);
  pinMode(RL_pin_num,OUTPUT);
  pinMode(RR_pin_num,OUTPUT);
 

  analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
}


