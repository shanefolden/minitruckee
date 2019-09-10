char serialData;
char receivedChars[140];
int pin = 13;
#define UPDATES_PER_SECOND 100
const byte numChars = 140;
//char receivedChars[numChars];   // an array to store the received data
char receivedChar;
boolean newData = false;
boolean x_direction = false; //boolean for input change in x/y dir or button pressed
boolean y_direction = false;
boolean button_pressed = false;
boolean left = false;
boolean right = false;
boolean forward = false;
boolean backward = false;
int FR_pin_num = 3;
int FL_pin_num = 5;
int RL_pin_num = 10;
int RR_pin_num = 11; 
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

  if(data[0]=='y') {
    y_direction = true;
  }
  else if(data[0]=='x') {
    x_direction = true;
  }

  else if(data[0]=='b') {
    button_pressed = true;
  }

  //remove first letter, convert rest of string into int 

  char *temp_string = data;
  temp_string++;
  int input_int;
  sscanf(temp_string, "%d", &input_int);
  float recent_x;
  float recent_y;
  // if joystick moved in y direction, determines direction + maps input_id to num between 0 and 255 for PWM
  if(y_direction){
    if(input_int < 50 && input_int >= 0){
      backward = true;
      forward = false;
      recent_y = ((float(input_int)-50.0)* (-1.0/50.0)); //0 is no backward thrust, 1 is full thrust
      recent_y = recent_y*255;//0 is no backward thrust, 255 is full thrust; this is the PWM value

    }

    else if(input_int >= 50 && input_int <= 100){
      backward = false;
      forward = true;

      recent_y=(float(input_int)-50.0)/50.0; //0 is no foward thrust, 1 is full forward thrust

      recent_y = recent_y*255; // o is no foward thrust, 255 is full thrust, this is the PWM value

    }
  }
  // if joystick moved in y direction, determines direction + maps input_id to num between 1/2 and 1 for steering multiplier
  if(x_direction){
    if(input_int <= 50 && input_int >= 0) {
      left = true;
      right = false;  
      recent_x = (float(input_int)*-1.0+50.0)/50.0;  // 50 = straight, 0 = full left, maps this to 0 is straight, 1 is full left
      Serial.print(" recent_x: ");
      Serial.println(recent_x); 
      recent_x = recent_x +1.0;  //1 is straight, 2 is full left
      recent_x = 1.0/recent_x; // 1 is straight, 1/2 is multiplier for full left

    }   
    else if(input_int > 50 && input_int <= 100){
      right = true;
      left = false;
      recent_x = (float(input_int)-50.0)/50.0; //maps 50= straight, 100 = full right; now 0 = straight, 1 = full right
      recent_x = 1.0/(1.0 + recent_x); // 1 is straight, 1/2 is multiplier for full right

    } 
  }
  // dont need these bools set true anymore, now more specific dir bool is true (left, right etc)
  x_direction = false;
  y_direction = false;
  
  //values passed to motor control fn
  int L_pwm_value;
  int R_pwm_value;

  if(left){
    L_pwm_value = round(recent_y*recent_x);
    R_pwm_value = round(recent_y);
  }
  else if(right){
    L_pwm_value = round(recent_y);
    R_pwm_value = round(recent_y*recent_x);
  }
  
  
  //Only call motor control if x or y input changes
  if(left || right || forward || backward) {
    motor_control(L_pwm_value,R_pwm_value);
  }

  else if(button_pressed)
  {
    button_control(input_int);
  }  


}

void motor_control(int left_pwm, int right_pwm) {
  //
  
  if(forward){
    //activate front left and front right motors
    analogWrite(FL_pin_num, left_pwm);
    analogWrite(FR_pin_num, right_pwm);
  }
  
  else if(backward){
    //activate back left + back right motors
    analogWrite(RL_pin_num, left_pwm);
    analogWrite(RR_pin_num, right_pwm);
  }


  // Once pin code is run, set all dir bools to false so they dont get run until theres a new input
  left = false;
  right = false;
  forward = false;
  backward = false;
}

void button_control(int buttonID){ 

  if(buttonID == 1){
    //A button pressed
    //do things
  }
  if(buttonID == 2){
    //B button pressed
    //do things
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








/* Not sure what these methods do but I'm scared to fully delete them
 void recvOneChar() {
 if (Serial.available() > 0) {
 receivedChar = Serial.read();
 newData = true;
 }
 }
 
 
 void showNewData() {
 if (newData == true) {
 Serial.print("test");  
 newData = false;
 }
 }
 
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


