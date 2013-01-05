/* Basic operation of single die. (D6)

(c) 2013 Chris McGinlay
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Requires Bounce library
http://playground.arduino.cc/Code/Bounce

LED layout: pin names and corresponding numbers
TL    TR    11    3
ML MC MR    10 4  5
BL    BR    9     6
*/

#include <Bounce.h>

const int ledTL = 11;
const int ledML = 10;
const int ledBL = 9;
const int ledBR = 6;
const int ledMR = 5;
const int ledTR = 3;
const int ledMC = 4;

const int btnRaise = 12;
const int btnLower = 7;
int stateRaise = HIGH;  //not pressed
int stateLower = HIGH;  //not pressed

const int default_state = 3;  //start visible
const int top_state = 6;  //make 7 for a D7
int cur_state = default_state;

Bounce bounceRaise = Bounce(btnRaise, 50); //50 millisecond
Bounce bounceLower = Bounce(btnLower, 50);

void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledTL, OUTPUT);
  pinMode(ledML, OUTPUT); 
  pinMode(ledBL, OUTPUT); 
  pinMode(ledBR, OUTPUT); 
  pinMode(ledMR, OUTPUT); 
  pinMode(ledTR, OUTPUT); 
  pinMode(ledMC, OUTPUT);

  pinMode(btnRaise, INPUT);
  pinMode(btnLower, INPUT);  
  
  cur_state = default_state;
}

void zero() {
  digitalWrite(ledTR, LOW); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, LOW); 
  digitalWrite(ledBL, LOW); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, LOW); 
  digitalWrite(ledMC, LOW);   
}

void one() {
  digitalWrite(ledTR, LOW); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, LOW); 
  digitalWrite(ledBL, LOW); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, LOW); 
  digitalWrite(ledMC, HIGH);   
}

void two() {
  digitalWrite(ledTR, LOW); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, LOW); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, LOW);   
}

void three() {
  digitalWrite(ledTR, LOW); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, LOW); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, HIGH);   
}

void four() {
  digitalWrite(ledTR, HIGH); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, HIGH); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, LOW);   
}

void five() {
  digitalWrite(ledTR, HIGH); 
  digitalWrite(ledMR, LOW); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, HIGH); 
  digitalWrite(ledML, LOW); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, HIGH);   
}

void six() {
  digitalWrite(ledTR, HIGH); 
  digitalWrite(ledMR, HIGH); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, HIGH); 
  digitalWrite(ledML, HIGH); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, LOW);   
}

void seven() {
  digitalWrite(ledTR, HIGH); 
  digitalWrite(ledMR, HIGH); 
  digitalWrite(ledBR, HIGH); 
  digitalWrite(ledBL, HIGH); 
  digitalWrite(ledML, HIGH); 
  digitalWrite(ledTL, HIGH); 
  digitalWrite(ledMC, HIGH);   
}

void (*states[8])() = { &zero, &one, &two, &three, &four, &five, &six, &seven };

void loop() {

  //explicit function call
/*
  zero();
  delay(1000);
  two();
  delay(1000);
*/

  //step through states using array of function pointers
/*
for (int loop=0; loop<=6; loop++) {  
    states[loop]();
    delay(2000);
  }
*/

  //one button raises number, the other reduces by one
  if (bounceRaise.update()) {
    stateRaise = bounceRaise.read();
    if (stateRaise==LOW && cur_state<top_state) {
      cur_state++;
    }
  }
  if (bounceLower.update()) {
    stateLower = bounceLower.read();
    if (stateLower==LOW && cur_state>1) {
      cur_state--;
    }
  }
  
  //press both buttons for random sequence
  if (stateLower == LOW && stateRaise == LOW) {
    for (int throws = 1; throws<10; throws++) {
      cur_state = random(1,top_state);
      states[cur_state]();
      delay(1000);
    }
  }     

  //display the state
  states[cur_state]();
}
