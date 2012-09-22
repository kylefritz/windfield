#include <Servo.h>

const int FANS              = 4;
const int FAN_1ST_RELAY_PIN = 3;
const int FAN_1ST_SERVO_PIN = 9;
const int ZERO_CROSS_PIN    = 2;
const int SERVO_0_DEG       = 40;
const int SERVO_360_DEG     = 140;

struct Fan{
  int relay;
  Servo servo;
};

Fan fans[FANS];

//state
int angle = 0;
int speed = 0;
int nthCross = 0;

/*
a-s (0-18)
a   0 0
d  60 3
j 180 9
n 260 13
s 360 18
*/

void blink(int ms)
{
  digitalWrite(13,LOW);
  delay(ms/2);
  digitalWrite(13,HIGH);
  delay(ms/2);
  digitalWrite(13,LOW);
}

void angleFans(){
  int servoPosition = constrain((angle/3.6)+SERVO_0_DEG,SERVO_0_DEG,SERVO_360_DEG);
   for(int i=0; i<FANS; i++){
    fans[i].servo.write(servoPosition);
  }
}
void switchFans(int mode){
   for(int i=0; i<FANS; i++){
    digitalWrite(fans[i].relay, mode);
   }
}

void setup(){
  Serial.begin(9600);

  for(int i=0; i<FANS; i++){
    fans[i].relay = i + FAN_1ST_RELAY_PIN;
    pinMode(fans[i].relay, OUTPUT);
    fans[i].servo.attach(i + FAN_1ST_SERVO_PIN);
  }
  angleFans();

  pinMode(ZERO_CROSS_PIN, INPUT);
  attachInterrupt(0, onZeroCross, CHANGE);

  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  blink(500);
  blink(500);
  blink(500);
}

void loop() {}

void serialEvent() {

  int _;
  while (Serial.available() < 2) { _++; }

  angle = 20*((int)Serial.read() - 'a');
  speed = (int)Serial.read() - 'a';

  Serial.print("angle ");
  Serial.print(angle);
  Serial.print(" speed ");
  Serial.println(speed);

  angleFans();
}

void onZeroCross()
{
  nthCross++;
//  digitalWrite(13, (nthCross % 2) ?HIGH:LOW);

  if(speed <= 0){
    switchFans(LOW);
  }
  else if(speed > 17)
  {
    switchFans(HIGH);
  }
  else if(speed < 4)
  {
    int nthWave = 11 - 2*speed; //(9,7,5,3)
    int mod = nthCross % nthWave;
    switchFans((mod==0||mod==1)?HIGH:LOW);
  }
  else //if(4 < speed <= 17)
  {
    int dimTime = 7000 - 500*(speed-4); //(7k-1k by 500)
    switchFans(LOW);
    delayMicroseconds(dimTime);
    switchFans(HIGH);
  }
}
