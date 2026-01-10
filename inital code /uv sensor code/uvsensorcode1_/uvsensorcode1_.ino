const int sensorPin = A2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(sensorPin);
  Serial.println(val);
  delay(500);
}
