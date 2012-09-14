#include <Bounce.h>

//http://www.youtube.com/watch?v=Ug4YE4k0CRM
//http://arduino.cc/forum/index.php/topic,111196.0.html


#define ZERO_CROSS 2
#define LED 13
#define DIM 12
#define BUTTON 3

int ledValue = LOW;
int dimTime = 4200;
int zerocross = 0;
boolean disableDimming = true;
boolean totallyOn = false;
boolean totallyOff = true;
Bounce bouncer = Bounce( BUTTON, 5 ); 

void setup() {
  pinMode(BUTTON,INPUT);  
  pinMode(LED, OUTPUT);     
  pinMode(DIM, OUTPUT);     
  digitalWrite(DIM, LOW);
  
  pinMode(ZERO_CROSS, INPUT);           // set pin to input
  attachInterrupt(0,zero, CHANGE);

  Serial.begin(9600);  
}

int period=16;

void zero() {
  zerocross++;
  if(!( (zerocross % period) == 0 || (zerocross % period) == 1 ) )
  {
    //ultra low
    return;
  }
  /*
  60 Hz current
  1/2 period is (1/120) sec = 8333 micro seconds
  */
  if(disableDimming) return;
  
  delayMicroseconds(dimTime);
  digitalWrite(DIM, HIGH);
  delayMicroseconds(8330 - 2 * dimTime);
  digitalWrite(DIM, LOW);
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
       
       dimTime -= 600;
       if(dimTime < 0){
         dimTime = 4200;
       }

       totallyOff = dimTime > 4100;
       totallyOn = dimTime == 0;
       
       disableDimming = totallyOff || totallyOn;
       
       if(totallyOn){
         digitalWrite(DIM, HIGH);
       }
       if(totallyOff){
         digitalWrite(DIM, LOW);
       }
       
       Serial.print(dimTime);
       Serial.print('\n');
     }
   }
}
