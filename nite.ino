#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// ---------- LCD PINS ----------
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ---------- GSM ----------


SoftwareSerial gsm(12, 13);
  
// ---------- IR SENSORS ----------
#define IR1 A0
#define IR2 A1 

// ---------- BUZZER ----------
#define BUZZER 12

// ---------- MOTOR DRIVER ----------
#define IN1 8
#define IN2 11
#define IN3 12
#define IN4 13

#define ENA 9
#define ENB 10

// ---------- MOTOR SPEED ----------
int M_SPEED = 0;   // 0–255 (90 LOW speed)

void setup() {

  // Display setup
  lcd.begin(16, 2);
  lcd.print("Railway Crack");
  lcd.setCursor(0,1);
  lcd.print("Detector Ready");
  delay(2000);
  lcd.clear();

  // IR pins
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Motor
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  gsm.begin(9600);
}

// --------------------------------------------------

void loop() {

  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);

  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(s1);
  lcd.print(" S2:");
  lcd.print(s2);

  // Both LOW => Track OK (white/normal)
  if (s1 == LOW && s2 == LOW) {
    lcd.setCursor(0,1);
    lcd.print("TRACK OK        ");
    digitalWrite(BUZZER, LOW);
    forwardSlow();
  }

  // One OR Both HIGH => Crack Found
  else {
    lcd.clear();
    lcd.print("CRACK DETECTED!");
    stopMotor();
    digitalWrite(BUZZER, HIGH);
    sendSMS("⚠️ Railway Track CRACK detected!");
    delay(3000);
    digitalWrite(BUZZER, LOW);
  }

  delay(100);
}

// --------------------------------------------------
// MOTOR CONTROL
// --------------------------------------------------

void forwardSlow() {
  analogWrite(ENA, M_SPEED);
  analogWrite(ENB, M_SPEED);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// --------------------------------------------------
// GSM MESSAGE
// --------------------------------------------------

void sendSMS(String msg) {
  gsm.println("AT+CMGF=1");
  delay(400);

  gsm.println("AT+CMGS=\"+91xxxxxxxxx\"");  // Replace your number
  delay(400);

  gsm.println(msg);
  delay(400);

  gsm.write(26); // CTRL+Z
  delay(3000);
}
