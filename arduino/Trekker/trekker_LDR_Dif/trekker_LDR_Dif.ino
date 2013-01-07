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
int enable12 = 5;  //bridge 1 and 2 enable (pin 1 on the 16 pin IC) 
                  //using PWM on this pin will control speed
int in1      = 3;  //in1 (out1 will be HIGH when set HIGH and LOW when set LOW) (pin 2 on the 16 pin IC)
int in2      = 4;  //in2 (out2 will be HIGH when set HIGH and LOW when set LOW) (pin 7 on the 16 pin IC)

//Motor two - LEFT
int enable34 = 9;
int in3      = 8;
int in4      = 7;

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
}

void loop(){
  delay(300);

  //motorRamp();

  level_Right = analogRead(LDR_rightPin);
  level_Left = analogRead(LDR_leftPin);

  //Compare left and right LDR readings
  float delta_dark = (level_Right-level_Left);
  delta_drive = map(delta_dark, -512, 512, -100, 100);

//  Serial.print(level_Right);
//  Serial.print("\t");
//  Serial.print(level_Left);
//  Serial.print("\t");
  Serial.print(delta_drive);
  Serial.println();
  
  if (delta_drive > 20) {
    freeRun();
    motorRightForward();
    Serial.print("VeerLeft");
  }
  else if (delta_drive < -20) {
    freeRun();
    motorLeftForward();
    Serial.print("VeerRight");
  } else {
    motorRightForward();
    motorLeftForward();
    Serial.print("Equal");
  }

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

void freeRun(){
 digitalWrite(in1, LOW); 
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW); 
}

void freeRunRight(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void freeRunLeft(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void motorRamp(){
  Serial.print("Ramping motors.\n");
  Serial.print("Speed:\t");
  motorLeftForward();
  motorRightForward();
  for (int i=50; i<=255;i+=10) {
    setSpeed(i,i);
    Serial.print(i);
    Serial.print("\t");
    delay(1000);
  }
  Serial.println("Ramp complete");
  freeRun();
}
   
