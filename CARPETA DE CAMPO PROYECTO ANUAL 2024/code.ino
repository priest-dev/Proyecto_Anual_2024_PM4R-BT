#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LedControlMS.h"

#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6
#define VELOCIDAD_ALTA 255
#define VELOCIDAD_BAJA 76

#define buzzer   10
#define Trigger  2  
#define Echo     3

byte Carita[8] = {
  B00000000,
  B00000100,
  B01110010,
  B00000010,
  B00000010,
  B01110010,
  B00000100,
  B00000000
};

byte CaritaC[8] = {
  B11000110,
  B10001111,
  B00101011,
  B00001111,
  B00001111,
  B00101011,
  B10001111,
  B11000110
};

byte CaritaE[8] = {
  B01000000,
  B01000001,
  B00101010,
  B00000010,
  B00000010,
  B00101010,
  B01000001,
  B01000000
};

long t;
long d;

LiquidCrystal_I2C lcd(0x27, 16, 2);
LedControl lc = LedControl(12, 11, 10, 1);

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
 
  lcd.init();
  lcd.backlight();
 
  for (int i = 0; i < 1; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void adelante() {
  analogWrite(IN1, VELOCIDAD_ALTA);
  analogWrite(IN2, 0);
  analogWrite(IN3, VELOCIDAD_ALTA);
  analogWrite(IN4, 0);
}

void reversa() {
  analogWrite(IN1, 0);
  analogWrite(IN2, VELOCIDAD_ALTA);
  analogWrite(IN3, 0);
  analogWrite(IN4, VELOCIDAD_ALTA);

  medirDistancia();
  mostrar_CaritaC();
}

void derecha() {
  analogWrite(IN1, VELOCIDAD_ALTA);
  analogWrite(IN2, 0);
  analogWrite(IN3, VELOCIDAD_BAJA);
  analogWrite(IN4, 0);
}

void izquierda() {
  analogWrite(IN1, VELOCIDAD_BAJA);
  analogWrite(IN2, 0);
  analogWrite(IN3, VELOCIDAD_ALTA);
  analogWrite(IN4, 0);
}

void loop() {
  reversa();
  delay(2000);
  adelante();
  delay(2000);
}

void medirDistancia() {
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);
  d = t / 59;

  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();
 
  lcd.setCursor(3, 0);
  lcd.print("Distancia");
  lcd.setCursor(1, 1);
  lcd.print(d);
  lcd.print(" Cm");
  delay(500);
  lcd.clear();
}

void mostrar_CaritaC() {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, CaritaC[i]);
  }
}

void mostrar_Carita() {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, Carita[i]);
  }
}

void mostrar_CaritaE() {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, CaritaE[i]);
  }