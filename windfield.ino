#include <Bounce.h>

//http://www.youtube.com/watch?v=Ug4YE4k0CRM
//http://arduino.cc/forum/index.php/topic,111196.0.html


#define ZERO_CROSS 2
#define LED 13
#define DIM 12
#define BUTTON 3

int ledValue = LOW;
int dim = 0;
Bounce bouncer = Bounce( BUTTON, 5 ); 

void setup() {
  pinMode(BUTTON,INPUT);  
  pinMode(LED, OUTPUT);     
  pinMode(DIM, OUTPUT);     
  
  pinMode(ZERO_CROSS, INPUT);           // set pin to input
  attachInterrupt(0,zero, CHANGE);

  Serial.begin(9600);  
}

void zero() {
  delayMicroseconds(65*dim);
  digitalWrite(DIM, HIGH);
//  digitalWrite(LED, HIGH);
  
  delayMicroseconds(1000);
  digitalWrite(DIM, LOW);
//  digitalWrite(LED, LOW);
}

void loop()
{
    if ( bouncer.update() ) {
     if ( bouncer.read() == HIGH) {
       
       if ( ledValue == LOW ) {
         ledValue = HIGH;
       } else {
         ledValue = LOW;
       }
       digitalWrite(LED,ledValue);
       
       dim+= 10;
       if(dim>128){
         dim=0;
       }
       Serial.print(dim);
       Serial.print('\n');
     }
   }
}
