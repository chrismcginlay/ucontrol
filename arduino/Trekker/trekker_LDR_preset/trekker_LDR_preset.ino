/*
  Trekker 1
  
  Line follower.
  Two drive motors, magnetically coupled
  Two LDR inputs, 1 LED source
 */
  

const int LDR_rightPin = 0; //analog0
const int LDR_leftPin = 1; //analog1

const int LEDPin = 13;

//L293 Pin assignments
//Motor one - RIGHT
int enable12 = 9;  //bridge 1 and 2 enable (pin 1 on the 16 pin IC) 
                  //using PWM on this pin will control speed
int in1      = 8;  //in1 (out1 will be HIGH when set HIGH and LOW when set LOW) (pin 2 on the 16 pin IC)
int in2      = 7;  //in2 (out2 will be HIGH when set HIGH and LOW when set LOW) (pin 7 on the 16 pin IC)

//Motor two - LEFT
int enable34 = 5;
int in3      = 3;
int in4      = 4;

int cruiseSpeed = 200;
int level_Right = 0;
int level_Left = 0;
int delta_drive = 0;

void setup(){
  delay(1000);
  pinMode(enable12, OUTPUT); 
  pinMode(enable34, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("LDR_right\tLDR_left\tdelta_drive");
  digitalWrite(LEDPin, HIGH); //Illumination on
  setSpeed(cruiseSpeed, cruiseSpeed);
  //motorLeftForward();
  //motorRightForward();
}

void loop(){
  delay(100);

  level_Right = analogRead(LDR_rightPin);
  level_Left = analogRead(LDR_leftPin);

  //Compare left and right LDR readings
  float delta_dark = (level_Right-level_Left);
  //map to [-128,127] for differential drive speed, centred on 128.
  delta_drive = map(delta_dark, -255, 255, -50, 50);
  
  Serial.print(level_Right);
  Serial.print("\t");
  Serial.print(level_Left);
  Serial.print("\t");
  Serial.print(delta_drive);
  Serial.println();
  
  setSpeed(cruiseSpeed-delta_drive, cruiseSpeed+delta_drive);
  motorLeftForward();
  motorRightForward();
}

////// Motor Routines  ////////////

void motorLeftForward(){
 digitalWrite(in1, LOW); 
 digitalWrite(in2, HIGH); 
}

void motorRightForward(){
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
}

//Unlikely that backwards motion will be required.
void motorLeftBackward(){
 digitalWrite(in1, HIGH); 
 digitalWrite(in2, LOW); 
}

void motorRightBackward(){
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

//drive both equally for straight ahead motion.
//Different speeds will produce steering.
void setSpeed(int newSpeedLeft, int newSpeedRight){
  analogWrite(enable12, newSpeedLeft);
  analogWrite(enable34, newSpeedRight);
}

void brake(){
 digitalWrite(in1, LOW); 
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW); 
}
   
