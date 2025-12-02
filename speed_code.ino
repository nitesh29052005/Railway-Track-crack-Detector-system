#include <LiquidCrystal.h>

// LCD Connection RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define sensor1 7
#define sensor2 8
#define buzzer 9

float distance = 0.05;  // 5 cm = 0.05 meter
int speedLimit = 50;    // km/hr overspeed limit

unsigned long t1 = 0, t2 = 0;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);

  lcd.print("Speed Detector");
  delay(1500);
  lcd.clear();
}

void loop() {

  // Wait until car crosses IR sensor 1
  if (digitalRead(sensor1) == LOW) {
    t1 = millis();

    // Wait until car reaches IR sensor 2
    while (digitalRead(sensor2) == HIGH);

    t2 = millis();

    float timeTaken = (t2 - t1) / 1000.0;  // seconds
    float speed = distance / timeTaken;    // m/s
    float speed_kmph = speed * 3.6;        // km/hr

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");
    lcd.print(speed_kmph, 1);
    lcd.print(" km/h");

    if (speed_kmph > speedLimit) {
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Over Speeding!");
    } else {
      digitalWrite(buzzer, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Speed Normal ");
    }

    delay(2000);
    digitalWrite(buzzer, LOW);
  }
}