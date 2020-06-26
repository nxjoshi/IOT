#define sensorPower 7
#define sensorPin A0
const int buzzer =  13;
int val=0;
int level=0;
void setup() {
  Serial.begin(115200);
  level = analogRead(A0);

  pinMode(buzzer, OUTPUT);
  digitalWrite (buzzer, LOW);
}


void loop(){
level = analogRead(A0);
Serial.println("Water Level is :");
Serial.println(level);
if(level>300){
   digitalWrite (buzzer, HIGH);
   Serial.println("In Buzzer :");
  }
else {
   digitalWrite (buzzer, LOW);
  }
}

//int readSensor() {
//  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
//  delay(10);              // wait 10 milliseconds
//  val = analogRead(sensorPin);    // Read the analog value form sensor
//  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
//  Serial.println();
//  return val;             // send current reading
//}
