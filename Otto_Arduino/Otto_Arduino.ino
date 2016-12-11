#include <Servo.h>
#include "Oscillator.h"

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
#define TRIM_YR 4
#define TRIM_YL -7

#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2

#define C 2100
#define D 1870
#define E 1670
#define f 1580
#define G 1400
#define R 0

#define INTERVALTIME 10.0 

Oscillator servo[N_SERVOS];

const int trigPin = 9;
const int echoPin = 8;
const int speakerOut = 10;

// function prototypes
void turnLeft(int steps, int T=3000);
void turnRight(int steps, int T=3000);
void walk(int steps, int T=1000);
void back(int steps, int T=3000);
bool safeDistance();
void playMusic();

int melody[] = {E,E,E,R,E,E,E,R,E,G,C,D,E,R,f,f,f,f,f,E,E,E,E,D,D,E,D};
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.
// Set overall tempo
long tempo = 7500;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 70; 
// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

void setup() {
  Serial.begin(115200);

  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);
  
  servo[0].SetTrim(TRIM_RR);
  servo[1].SetTrim(TRIM_RL);
  servo[2].SetTrim(TRIM_YR);
  servo[3].SetTrim(TRIM_YL);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerOut, OUTPUT);
  
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
        } else {
          tone(10, 440, 1000);
        }
        break;
      case 'B':
        Serial.println("MOVING OTTO DOWN");
        tone(13, 800, 100);
        back(2, t*2);
        break;
      case 'L':
        Serial.println("MOVING OTTO LEFT");
        turnLeft(2, t*2);
        break;
      case 'R':
        Serial.println("MOVING OTTO RIGHT");
        turnRight(2, t*2);
        break;
      case 'M':
        Serial.println("PLAYING MUSIC");
        playMusic();
        break;
    }
  }
}

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

void back(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void walk(int steps, int T){
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
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Make sure not sending any sound
  delayMicroseconds(2); // Clears the speaker for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Start sound waves
  delayMicroseconds(10); // Run for 10 microseconds
  digitalWrite(trigPin, LOW); // Turn off sound waves
  duration = pulseIn(echoPin, HIGH);  // Set the echo pin to wait till receive 5V
  distance = (duration/2) * 0.03435;  // Calculate the distance by formula (Distance = Speed * Time)
                                      // Time / 2 because do not want the time from the object and back
                                      // We only want the time to the object.
                                      // 0.03435 is the speed of sound in CM/S^2
  return (distance > 5);
}

void playTone() 
{
   long elapsed_time = 0;
   if(tone_ > 0) { // if this isn't a Rest beat, while the tone has
     //  played less long than 'duration', pulse speaker HIGH and LOW
   while(elapsed_time < duration) {
       digitalWrite(speakerOut,HIGH);
       delayMicroseconds(tone_ / 2);
       // DOWN
       digitalWrite(speakerOut, LOW);
       delayMicroseconds(tone_ / 2);
       // Keep track of how long we pulsed
       elapsed_time +=(tone_);
     }
   }
   else 
   { 
    // Rest beat; loop times delay
     for(int j = 0; j < rest_count; j++) 
   {
       delayMicroseconds(duration); 
     } 
   }                                
}

void playMusic() {
    for(int i=0; i < MAX_COUNT; i++) {
    tone_ = melody[i];
    beat = 50;
    duration = beat * tempo; // Set up timing
    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
    }
}
