int nthCross;
const int ZERO_CROSS_PIN=2;

void setup(){
  Serial.begin(115200);

  pinMode(ZERO_CROSS_PIN, INPUT);
  attachInterrupt(0, zero, CHANGE);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
//  delay(6000);//wait for servos to get into position
}

void loop() {}

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

  Serial.print("cross ");
  Serial.print(nthCross);
  Serial.print('\n');
}

void zero()
{
  digitalWrite(13, (nthCross % 2) ?HIGH:LOW);

  //handle the skipping of periods
  nthCross++;
}
