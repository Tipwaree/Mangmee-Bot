#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define SERVO_PIN 9

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte closesmile1[8] = {
  B00000, B00001, B00011, B00111, B01110, B01100, B11100, B11100
};
byte closesmile2[8] = {
  B11100, B11100, B01100, B01110, B00111, B00011, B00001, B00000
};
byte closesmile3[8] = {
  B00000, B00000, B00000, B00000, B11111, B11111, B11111, B00000
};
byte closesmile4[8] = {
  B00111, B00111, B00110, B01110, B11100, B11000, B10000, B00000
};
byte closesmile5[8] = {
  B00000, B11111, B11111, B11111, B00000, B00000, B00000, B00000
};
byte closesmile6[8] = {
  B00000, B10000, B11000, B11100, B01110, B00110, B00111, B00111
};

long duration;
float distance;
bool lidOpen = false;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  myServo.write(90);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.createChar(1, closesmile1);
  lcd.createChar(2, closesmile2);
  lcd.createChar(3, closesmile3);
  lcd.createChar(4, closesmile4);
  lcd.createChar(5, closesmile5);
  lcd.createChar(6, closesmile6);

  /*lcd.setCursor(0, 0);
  lcd.print("Smart Trash Bin");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!");
  delay(1500);
  lcd.clear();*/
}

void loop() {
  // วัดระยะ
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ฝาเปิด
  if (distance > 2 && distance <= 10 && !lidOpen) {
    myServo.write(0);
    lidOpen = true;

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Mmm Yummy :)");
    delay(2000);
  }
  // ฝาปิด
  else if ((distance > 10 || distance <= 0) && lidOpen) {
    myServo.write(90);
    lidOpen = false;
    lcd.clear();
  }

  // ขยับปากซ้ำๆ ระหว่างฝาปิด
  if (!lidOpen) {
    drawMouthClosed();
    delay(500);
    drawMouthOpen();
    delay(500);
  }
}

void drawMouthClosed() {
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.write(byte(2));
  lcd.setCursor(5, 1);
  for (int i = 0; i < 6; i++) lcd.write(byte(3));
  lcd.setCursor(11, 1);
  lcd.write(byte(4));
}

void drawMouthOpen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write(byte(1));
  lcd.setCursor(5, 0);
  for (int i = 0; i < 6; i++) lcd.write(byte(5));
  lcd.setCursor(11, 0);
  lcd.write(byte(6));

  lcd.setCursor(4, 1);
  lcd.write(byte(2));
  lcd.setCursor(5, 1);
  for (int i = 0; i < 6; i++) lcd.write(byte(3));
  lcd.setCursor(11, 1);
  lcd.write(byte(4));
}
