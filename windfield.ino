#include <Servo.h>

struct Fan{
  int relay; //which relay pin
  Servo servo;
};

const int FANS = 4;
const int FAN_1ST_RELAY_PIN=30;
const int FAN_1ST_SERVO_PIN=9;
const int ZERO_CROSS_PIN=2;

Fan fans[FANS];

const int SERVO_0_DEG= 40;
const int SERVO_360_DEG = 140;

//fan state
volatile int angle = 0;
volatile int speed = 0;

void setup(){
  Serial.begin(115200);

  //servo
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
}

void loop() {}

/*
 * send in F<fan number>A<angle>S<speed>
 */

void serialEvent() {
  char firstChar = Serial.read();

  if(firstChar != 'a'){
    Serial.print("don't understand: ");
    Serial.print(firstChar);
    while (Serial.available() > 0) {
      Serial.write(Serial.read());
    }
    Serial.print("\n");
    return;
  }
  
  angle = Serial.parseInt();
  speed = Serial.parseInt();
  
  Serial.print("angle ");
  Serial.print(angle);
  Serial.print(" speed ");
  Serial.println(speed); 
  
  angleFans();
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

int nthCross;
void onZeroCross()
{
  nthCross++;
  digitalWrite(13, (nthCross % 2) ?HIGH:LOW);
  
  if(speed==0){
    switchFans(LOW);
  }
  else if(speed > 10)
  {
    switchFans(HIGH);
  }
  else if(speed < 4)
  {
    int nthWave = 11 - 2*speed; //(9,7,5,3)
    int mod = nthCross % nthWave;
    switchFans((mod==0||mod==1)?HIGH:LOW);
  }
  else
  {
    //if(4 < speed <= 10)
    int dimTime = 3500 - 500*(speed-4); //(3.5k, 3k, 2.5k, 2k, 1.5k, 1k)
    switchFans(LOW);
    delayMicroseconds(dimTime);
    switchFans(HIGH);
  }
}
