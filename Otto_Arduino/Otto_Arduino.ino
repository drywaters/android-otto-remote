/*const int BUZZER_PIN = 13;


void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    
    
    

}

void loop() {


  

  
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    char character = Serial.read();
    switch(character) {
      case 'U':
        Serial.println("MOVING OTTO UP");
        break;
      case 'D':
        Serial.println("MOVING OTTO DOWN");
        break;
      case 'L':
        Serial.println("MOVING OTTO LEFT");
        break;
      case 'R':
        Serial.println("MOVING OTTO RIGHT");
        break;
    }
  }
}
*/

#include "note_definition.h"

int speakerOut = 9;
bool played = false;

long melody[] = { E5, E5, E5, E5, E5, E5, E5, G5, C5, D5, E5, F5, F5, F5, F5, F5, E5, E5, E5, E5, D5, D5, E5, D5, G5, E5, E5, E5, E5, E5, E5, E5, G5, C5, D5, E5, F5, F5, F5, F5, F5, E5, E5, E5, G5, G5, F5, D5, C5 };
int beats[]  =  {  8,  8, 16,  8,  8, 16,  8,  8, 12,  4, 32,  8,  8, 12,  4,  8,  8,  8,  8,  8,  8,  8,  8, 16, 16,  8,  8, 16,  8,  8, 16,  8,  8, 12,  4, 32,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 32 }; 

int MAX_COUNT = sizeof(melody) / sizeof(long); // Melody length, for looping.

// Set overall tempo
long tempo = 50000;

// Set length of pause between notes
long pause = 100000;

// Loop variable to increase Rest length
int rest_count = 100; 

// Initialize core variables
long tone_ = 0;
int beat = 0;
long duration  = 0;


void playMusic() {
  
  for (int i=0; i<MAX_COUNT; i++) {
   
    tone_ = melody[i];
    beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone(); 
    // A pause between notes...
    delayMicroseconds(pause);
    }
  
}

void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}

void setup() { 
  
  pinMode(speakerOut, OUTPUT);

  Serial.begin(115200);
  
}

void loop() {

  if(Serial.available()) {
    char character = Serial.read();
    switch(character) {
      case 'U':
        Serial.println("MOVING OTTO UP");
        break;
      case 'D':
        Serial.println("MOVING OTTO DOWN");
        break;
      case 'L':
        Serial.println("MOVING OTTO LEFT");
        break;
      case 'R':
        Serial.println("MOVING OTTO RIGHT");
        break;
      case 'M':
        Serial.println("PLAYING MUSIC");
        playMusic();
        break;
    }
  }
}
