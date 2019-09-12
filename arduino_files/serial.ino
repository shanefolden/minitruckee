char serialData;
char receivedChars[140];
int pin = 13;
#define UPDATES_PER_SECOND 100
const byte numChars = 140;
//char receivedChars[numChars];   // an array to store the received data
char receivedChar;
boolean newData = false;

boolean change_in_dir = false;
boolean button_pressed = false;

boolean first_input = false;
int FR_pin_num = 6;
int FL_pin_num = 9;
int RL_pin_num = 5;
int RR_pin_num = 3; 
float recent_x;
float recent_y;
void setup() { 
  Serial.begin(115200);

  pinMode(FR_pin_num,OUTPUT);
  pinMode(FL_pin_num,OUTPUT);
  pinMode(RL_pin_num,OUTPUT);
  pinMode(RR_pin_num,OUTPUT);
    
}

void loop()
{

  String content = "";
  char character;
  static byte ndx = 0;
  char endMarker = '>'; //char that signifies end of transmitted data
  char rc;

  while (Serial.available() > 0 && newData == false) {
  if(!first_input) {
     //delay(3000);
     first_input = true;
  }
    //reads serial input, adds to receivedChars
    rc = Serial.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;

      ndx++;
      content.concat(rc);
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else { //once endMarker found,
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      //Now has new transmitted Data
      newData = true;
    }
  }
  //can prob delete this if statement and put it in else statement above ^^
  if (newData) {
    // lights(receivedChars);
    analyzeString(receivedChars);   
    newData=false;
  }
 
}


void analyzeString(char data[])
{
  //func that deconstructs string, extracts info, calls motor or button func 
  //expected string format: lnn
  //l is letter id for x dir, y dir or button
  //n is amount joystick pressed in, or button id

  /* formula to map one range of numbers to the other:
   y = new number
   x = old number
   [a-b] = old range
   [c-d] = new range
   y = (x-a) * ((d-c)/(b-a)) + c
   */

  //determine first letter and set correct bool

  if(data[0]=='d') {
    change_in_dir = true;
  }

  else if(data[0]=='b') {
    button_pressed = true;
  }

  //remove first letter, convert rest of string into int 

  char *temp_string = data;
  temp_string++;
  int input_int;
  sscanf(temp_string, "%d", &input_int);
  
  if(change_in_dir) {
      motor_control(input_int);
  }
  else if(button_pressed)
  {
    button_control(input_int);
  }
  change_in_dir = false;  
}

void motor_control(int dir_value) {
  
  int left_pwm = 0;
  int right_pwm = 0;
  
  if(dir_value==0){
    //activate front left and front right motors
    analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
}
  else if(dir_value==1){
    analogWrite(FL_pin_num, 50);
    analogWrite(FR_pin_num, 200);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
  }
   else if(dir_value==2){
    analogWrite(FL_pin_num, 200);
    analogWrite(FR_pin_num, 200);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
  }
   else if(dir_value==3){
    analogWrite(FL_pin_num, 200);
    analogWrite(FR_pin_num, 50);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
  }

   else if(dir_value==4){
    analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 50);
    analogWrite(RR_pin_num, 200);
  }

   else if(dir_value==5){
    analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 200);
    analogWrite(RR_pin_num, 200);
  }

   else if(dir_value==6){
    analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 200);
    analogWrite(RR_pin_num, 50);
  }
  
}

void button_control(int buttonID){ 

  if(buttonID == 1){
    //A button pressed
    //do things
  }
  if(buttonID == 2){
    //B button pressed
    //Emergency stop - stop all motors and sleep for 3s
    analogWrite(FL_pin_num, 0);
    analogWrite(FR_pin_num, 0);
    analogWrite(RL_pin_num, 0);
    analogWrite(RR_pin_num, 0);
    delay(3000);

  } 
  if(buttonID == 3){
    //X button pressed
    //do things
  } 
  if(buttonID == 4){
    //Y button pressed
    //do things
  }  
  //sets button to false that way this method doesnt run again until new button input 
  // because of the way its coded, all button actions must be toggled, not hold and release
  button_pressed = false;  
}

//End of File, only commented sections below





/* 
 Test print statements and func calls that might be useful later:
 
 Serial.print("  x = 0:  ");
 analyzeString("x0>");
 Serial.print("  x = 1:  ");
 analyzeString("x1>");
 Serial.print("  x = 10:  ");
 analyzeString("x10>");
 Serial.print("  x = 20:  ");
 analyzeString("x20>");
 Serial.print("  x = 30:  ");
 analyzeString("x30>");
 Serial.print("  x = 40:  ");
 analyzeString("x40>");
 Serial.print("  x = 50:  ");
 analyzeString("x50>");
 
 Serial.print("  x = 60:  ");
 analyzeString("x60>");
 Serial.print("  x = 70:  ");
 analyzeString("x70>");
 Serial.print("  x = 80:  ");
 analyzeString("x80>");
 Serial.print("  x = 90:  ");
 analyzeString("x90>");
 Serial.print("  x = 100:  ");
 analyzeString("x100>");
 
 */
