// ---------- PIN SETUP ----------
const int ledPin = 13;      // UV LED pin 
const int sensorPin = A2;   // Photodiode / phototransistor pin


int readUV() {
  long total = 0;
  for (int i = 0; i < 20; i++) {
    total += analogRead(sensorPin);
    delay(5);
  }
  return total / 20;
}

// ---------- VARIABLES ----------
int baseline = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Turn on UV LED
  digitalWrite(ledPin, HIGH);

  Serial.println("UV Sunscreen Tester");
  Serial.println("-----------------------");
  Serial.println("Insert CLEAN slide.");
  Serial.println("Press ENTER when ready to record baseline.");
}

void loop() {
  baseline = readUV();    // record clean slide reading

  Serial.print("Baseline recorded: ");
  Serial.println(baseline);
  Serial.println("Now insert sunscreen slide...");
  Serial.println("-----------------------------");
  delay(1000);


}
