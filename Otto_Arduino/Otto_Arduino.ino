#include <Servo.h>
#include "Oscillator.h"
#include <EEPROM.h>

#define N_SERVOS 4
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
#define TRIM_RR 7
#define TRIM_RL 4
#define TRIM_YR  4
#define TRIM_YL -7
//OTTO.setTrims(-7,-4,-4,7);

#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2

#define INTERVALTIME 10.0 

Oscillator servo[N_SERVOS];

const int trigPin = 8;
const int echoPin = 9;

long duration,cm;


// function prototypes
void turnLeft(int steps, int T=3000);
void turnRight(int steps, int T=3000);
void walk(int steps, int T=1000);
void backyard(int steps, int T=3000);
bool safeDistance();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);
  
  servo[0].SetTrim(TRIM_RR);
  servo[1].SetTrim(TRIM_RL);
  servo[2].SetTrim(TRIM_YR);
  servo[3].SetTrim(TRIM_YL);
  
  for(int i=0;i<4;i++) servo[i].SetPosition(90);
}

// TEMPO: 121 BPM
int t=495;

void loop() {
  
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    char character = Serial.read();
    switch(character) {
      case 'F':
        Serial.println("MOVING OTTO UP");
        if (safeDistance()) {
          walk(2, t*2);
        }
        break;
      case 'B':
        Serial.println("MOVING OTTO DOWN");
        backyard(2, t*2);
        break;
      case 'L':
        Serial.println("MOVING OTTO LEFT");
        turnLeft(2, t*2);
        break;
      case 'R':
        Serial.println("MOVING OTTO RIGHT");
        turnRight(2, t*2);
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////FUNCIONES DE CONTROL//////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void oscillate(int A[N_SERVOS], int O[N_SERVOS], int T, double phase_diff[N_SERVOS]){
  for (int i=0; i<4; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<T+ref; x=millis()){
     for (int i=0; i<4; i++){
        servo[i].refresh();
     }
  }
}

void walk(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void backyard(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void turnLeft(int steps, int T){
    int A[4]= {20, 20, 10, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void turnRight(int steps, int T){
    int A[4]= {20, 20, 30, 10};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

bool safeDistance() {
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = duration / 29 / 2;
  return cm > 5;
}

