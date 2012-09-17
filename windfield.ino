//internal pull up: http://arduino.cc/en/Tutorial/InputPullupSerial

struct Fan{
  int angle;
  int speed;
  int wait;  //how long it attenuate, 0 means don't attenuate by wait
  int mod;   //how many periods it should skip. 1 = none (always on); 0 means don't attenuate by period
  //if wait==0 and mod==0, fan off
  int relay; //which relay pin
};

const int FANS = 4;
const int FAN_STARTING_PIN=30;

Fan fans[FANS];
Fan fansByWait[FANS]; //every time you change a fan speed, recreate/reset this array

void setup(){
  Serial.begin(115200);
  for(int i=0; i<FANS; i++){
    fans[i].relay= i + FAN_STARTING_PIN;
  }
}


void loop() {
}

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
  fans[fan].speed = speed;

  printFans();
  updateFansByWait();
  printFanWaits();
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
  //handle the skipping of periods
  nthCross++;
  for(int i=0; i<FANS; i++){
    Fan fan = fans[i];
    boolean enable =  fan.mod != 0
                      && (nthCross % fan.mod == 0); /*TODO: also mod 1?*/
    digitalWrite(fan.relay, enable);
  }

  //handle wait times
  int totalDelay = 0;
  for(int i=0; i<FANS; i++){
    Fan fan = fansByWait[i];
    if(fan.wait == 0) continue;
    int delayTime = fan.wait - totalDelay;
    if(delayTime > 0){
      delay(delayTime);
      totalDelay += delayTime;
    }
    digitalWrite(fan.relay, HIGH);
  }
}
