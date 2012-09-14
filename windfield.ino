#include <Bounce.h>

//http://www.youtube.com/watch?v=Ug4YE4k0CRM
//http://arduino.cc/forum/index.php/topic,111196.0.html


#define ZERO_CROSS 2
#define LED 13
#define DIM 12
#define BUTTON 3

int ledValue = LOW;
int zerocross = 0;
Bounce bouncer = Bounce( BUTTON, 5 ); 
int period = 25;

void setup() {
  pinMode(BUTTON,INPUT);  
  pinMode(LED, OUTPUT);     
  pinMode(DIM, OUTPUT);     
  digitalWrite(DIM, LOW);
  
  pinMode(ZERO_CROSS, INPUT);           // set pin to input
  attachInterrupt(0,zero, CHANGE);

  Serial.begin(9600);  
}


void zero() {
  zerocross++;
  if((zerocross % period) == 0 || (zerocross % period) == 1 )
  {
    //ultra low
    digitalWrite(DIM, HIGH);
  }else{
    digitalWrite(DIM, LOW);    
  }
  /*
  60 Hz current
  1/2 period is (1/120) sec = 8333 micro seconds
  */
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
       
       period -= 1;
       if(period < 0){
         period = 25;
       }
       
       Serial.print(period);
       Serial.print('\n');
     }
   }
}
