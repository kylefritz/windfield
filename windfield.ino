#include <Servo.h>

struct Fan{
  int angle;
  int speed;
  int wait;  //how long it attenuate, 0 means don't attenuate by wait
  int mod;   //how many periods it should skip. 1 = none (always on); 0 means don't attenuate by period
  //good values for mod: 9, 7, 5, 3
  //if wait==0 and mod==0, fan off
  int relay; //which relay pin
  Servo servo;
  int servoPosition; //the actual number sent to the servo
};

const int FANS = 4;
const int FAN_1ST_RELAY_PIN=30;
const int FAN_1ST_SERVO_PIN=9;
const int ZERO_CROSS_PIN=2;

Fan fans[FANS];
Fan fansByWait[FANS]; //every time you change a fan speed, recreate/reset this array

const int SERVO_0_DEG= 40;
const int SERVO_360_DEG = 140;
const int MS_PER_DEG = 60;

void setup(){
  Serial.begin(115200);

  for(int i=0; i<FANS; i++){
    fans[i].relay = i + FAN_1ST_RELAY_PIN;
    pinMode(fans[i].relay, OUTPUT);
    fans[i].servo.attach(i + FAN_1ST_SERVO_PIN);
    fans[i].servo.write(SERVO_0_DEG);
    fans[i].servoPosition = SERVO_0_DEG;
  }
  
  pinMode(ZERO_CROSS_PIN, INPUT); 
  attachInterrupt(0, onZeroCross, CHANGE);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  
//  digitalWrite(fans[0].relay, HIGH);
//  delay(6000);//wait for servos to get into position
}

void loop() {}

/*
 * send in F<fan number>A<angle>S<speed>
 */

void serialEvent() {
  char firstChar = Serial.read();

  if(firstChar != 'F'){
    Serial.print("don't understand: ");
    Serial.print(firstChar);
    while (Serial.available() > 0) {
      Serial.write(Serial.read());
    }
    Serial.print("\n");
    return;
  }

  int fan = Serial.parseInt();
  int angle = Serial.parseInt();
  int speed = Serial.parseInt();

  fans[fan].angle = angle;
  int servoPosition = constrain((angle/3.6)+SERVO_0_DEG,SERVO_0_DEG,SERVO_360_DEG);
  fans[fan].servoPosition = servoPosition;
  fans[fan].servo.write(servoPosition);
  fans[fan].speed = speed;
  fans[fan].mod = speed;

  printFans();
  updateFansByWait();
//  printFanWaits();
  Serial.print('\n');
}

void printFans(){
  for(int i=0; i<FANS; i++){
    Fan fan = fans[i];
    Serial.print("Fan ");
    Serial.print(i);
    Serial.print(" relay ");
    Serial.print(fan.relay);
    Serial.print(" angle ");
    Serial.print(fan.angle);
    Serial.print(" pos ");
    Serial.print(fan.servoPosition);
    Serial.print(" speed ");
    Serial.print(fan.speed);
    Serial.print(" wait ");
    Serial.print(fan.wait);
    Serial.print(" mod ");
    Serial.println(fan.mod);
  }
}

void printFanWaits(){
  for(int i=0; i<FANS; i++){
    Fan fan = fansByWait[i];
    Serial.print("Fan ");
    Serial.print(fan.relay);
    Serial.print(" wait ");
    Serial.println(fan.wait);
  }
}


void updateFansByWait()
{
  //copy vals
  for(int i=0; i<FANS; i++)
  {
    fansByWait[i] = fans[i];
  }

  //bubble sort
  for (int i = 1; i < FANS; ++i)
  {
    Fan j = fansByWait[i];
    int k;
    for (k = i - 1; (k >= 0) && (j.wait < fansByWait[k].wait); k--)
    {
      fansByWait[k + 1] = fansByWait[k];
    }
    fansByWait[k + 1] = j;
  }
}

int nthCross;
void onZeroCross()
{
  digitalWrite(13, (nthCross % 2) ?HIGH:LOW);
  
  //handle the skipping of periods
  nthCross++;
  for(int i=0; i<FANS; i++){
    Fan fan = fans[i];
    if(fan.mod == 0) continue;
    
    int mod = nthCross%fan.mod;
    boolean enable =  mod==0||mod==1;
    digitalWrite(fan.relay, (mod==0||mod==1)?HIGH:LOW);
  }

  //handle wait times
//  int totalDelay = 0;
//  for(int i=0; i<FANS; i++){
//    Fan fan = fansByWait[i];
//    if(fan.wait == 0) continue;
//    int delayTime = fan.wait - totalDelay;
//    if(delayTime > 0){
//      delay(delayTime);
//      totalDelay += delayTime;
//    }
//    digitalWrite(fan.relay, HIGH);
//  }
}
