/* Robot Arm Controller 

Copyright (c) 2013 Chris McGinlay

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

*/

#define __ASSERT_USE_STDERR
#include <Servo.h>
#include <assert.h>

//Objects to manage average of n most recent states
class Smoother {

private:
  static const int MAXSAMPLES = 20; //hard array limit 
  int idx;  //index of current reading
  int count;  //1 indexed. Number of samples added
  int n_samples;  //number of samples to average (soft limit)
  int samples[MAXSAMPLES-1];  //array of samples, 20 max

public:
  Smoother(int ns = 10); //default 10 samples
  int getVal();  //return current average
  int update(int reading);  //update sample array, return avg.
};

Smoother::Smoother(int ns) {
  n_samples = ns;
  assert (n_samples<MAXSAMPLES);
  assert (n_samples>0);
  
  //Proper testing should mean this test never passes
  if (n_samples>MAXSAMPLES) {
    n_samples = MAXSAMPLES;
  }
  for (int i=0; i<n_samples-1; i++) {
    samples[i] = 0;
  }
  idx = 0;   //set index to start
  count =0;
}
 
int Smoother::getVal() {
  assert(count>0);
  assert(count<=n_samples);
  int total =0;
  int average = 0;  //arithmetic mean
  for (int i=0; i<count; i++) {
    total += samples[i];
  }
  average = total / count;
  return average;
}

int Smoother::update(int reading) {
  assert (idx<n_samples);
  idx++;
  if (idx==n_samples) {
      idx=0;
  }
  samples[idx] = reading;
  if (count<n_samples) count++;
  return Smoother::getVal();
}

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
  Serial.println(__func);
  Serial.println(__file);
  Serial.println(__lineno, DEC);
  Serial.println(__sexp);
  Serial.flush();
  abort();
}

Servo ser1, ser2, ser3, ser4;
Smoother pos1, pos2, pos3, pos4;

int control1 = 0;  //raw control data from voltage dividers
int control2 = 0;
int control3 = 0;
int control4 = 0;

int c1m = 0;  //mapped control inputs
int c2m = 0;
int c3m = 0;
int c4m = 0; 

int VPin1 = A0;  //voltage divider pin allocations
int VPin2 = A1;
int VPin3 = A2;
int VPin4 = A3;

int SPin1 = 9;  //PWM servo pin allocations
int SPin2 = 6; 
int SPin3 = 5;
int SPin4 = 3;

void showServos() {
  Serial.print(c1m);
  Serial.print(" ");
  Serial.print(c2m);
  Serial.print(" ");
  Serial.print(c3m);
  Serial.print(" ");
  Serial.println(c4m);
  return;
}
  
void setup() {
  Serial.begin(9600);
  Serial.println("Robot Arm Control");
  Serial.flush();
  ser1.attach(SPin1);
  ser2.attach(SPin2);
  ser3.attach(SPin3);
  ser4.attach(SPin4);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  control1 = pos1.update(analogRead(VPin1));
  control2 = pos2.update(analogRead(VPin2));
  control3 = pos3.update(analogRead(VPin3));
  control4 = pos4.update(analogRead(VPin4));
  
  c1m = map(control1, 50, 1000, 200, 0);
  c2m = map(control2, 50, 1000, 200, 0);
  c3m = map(control3, 50, 1000, 200, 0);
  c4m = map(control4, 50, 1000, 200, 0);

  ser1.write(c1m);
  ser2.write(c2m);
  ser3.write(c3m);
  ser4.write(c4m);
  
  showServos();
  delay(20);
}
