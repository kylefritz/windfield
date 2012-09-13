#include <Bounce.h>

//http://www.youtube.com/watch?v=Ug4YE4k0CRM

// This code turns a led on/off through a debounced button
// Build the circuit indicated here: http://arduino.cc/en/Tutorial/Button

#define BUTTON 2
//#define ZERO_CROSS 3
#define LED 13

Bounce bouncer = Bounce( BUTTON,5 ); 

void setup() {                
  pinMode(LED, OUTPUT);     
  pinMode(BUTTON,INPUT);
  
//  pinMode(ZERO_CROSS, INPUT);           // set pin to input
//  digitalWrite(ZERO_CROSS, HIGH);       // turn on pullup resistors
}

void loop() {
  bouncer.update();//updates the state and stores for later
  
 // Turn on or off the LED
 if ( bouncer.read() == HIGH) {
   digitalWrite(LED, HIGH );
 } else {
    digitalWrite(LED, LOW );
 }
}

