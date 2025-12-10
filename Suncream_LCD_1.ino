#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"

Waveshare_LCD1602_RGB lcd(16, 2);   

const int sensorPin  = A2;   


int readUV() {
  long sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += analogRead(sensorPin);
    delay(5);
  }
  return sum / 30;
}

int baseline = 0;  // clean slide value

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("UV Sunscreen Tester");
  Serial.println("--------------------");
  Serial.println("1) Insert CLEAN slide.");
  Serial.println("2) Wait for baseline to be saved.");
  Serial.println("3) Then insert SUNSCREEN slide.");
  Serial.println();

  // LCD INIT
  lcd.init();
  int r = 255, g = 255, b = 255; //brghtness
  lcd.setRGB(r, g, b);

  lcd.setCursor(0, 0);
  lcd.send_string("UV Tester Ready");

  lcd.setCursor(0, 1);
  lcd.send_string("Clean slide...");
}

void loop() {

  // RECORD BASELINE 
  while (baseline == 0) {
    int reading = readUV();

   
    Serial.print("Baseline reading: ");
    Serial.println(reading);

    // LCD line 1: show baseline live
    lcd.setCursor(0, 1);
    char line[17];
    snprintf(line, sizeof(line), "Base:%4d       ", reading);
    lcd.send_string(line);

    static unsigned long start = millis();
    if (millis() - start > 3000) {  
      baseline = reading;

      Serial.println();
      Serial.print("Baseline saved: ");
      Serial.println(baseline);
      Serial.println("Now insert SUNSCREEN slide.");
      Serial.println();

      // Show message on LCD
      lcd.setCursor(0, 0);
      lcd.send_string("Baseline Saved  ");
      lcd.setCursor(0, 1);
      lcd.send_string("Sunscreen slide ");
      delay(1500);

      // Clear slide
      lcd.setCursor(0, 0);
      lcd.send_string("                ");
      lcd.setCursor(0, 1);
      lcd.send_string("                ");
    }
  }

  //  MEASURE SUNSCREEN UV BLOCKING 
  int current = readUV();
  if (baseline < 5) baseline = 5;

  float transmission = (float)current / baseline * 100.0;
  if (transmission > 100) transmission = 100;

  float blocked = 100.0 - transmission;

  // Serial debug
  Serial.print("Current: ");
  Serial.print(current);
  Serial.print("  |  UV Block: ");
  Serial.print(blocked, 1);
  Serial.println("%");

// DISPLAY ON LCD 

// Line 0: UV detected 
char line0[17];
snprintf(line0, sizeof(line0), "UV Detected:%4d", current);
lcd.setCursor(0, 0);
lcd.send_string(line0);

// Convert blocked (float) to integer %
int blockedInt = (int)(blocked + 0.5);  // round to nearest
if (blockedInt < 0)   blockedInt = 0;
if (blockedInt > 100) blockedInt = 100;

// Clear line 1 first
lcd.setCursor(0, 1);
lcd.send_string("                ");  
// Now print: "UV Blocked:XX%"
lcd.setCursor(0, 1);
lcd.send_string("UV Blocked:");

// Convert number to text
char percentBuf[5];
itoa(blockedInt, percentBuf, 10); 

// Print the number
lcd.send_string(percentBuf);

// Print the % sign
lcd.send_string("%");


  
  delay(200);
}

