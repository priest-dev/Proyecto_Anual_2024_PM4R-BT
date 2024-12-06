#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LedControlMS.h"
#include <SoftwareSerial.h>

#define IN1 3
#define IN2 5
#define IN3 6
#define IN4 9
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
SoftwareSerial bluetooth(7, 8);

#define NOTE_B3  247
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_A4  440
#define NOTE_B4  494

#define NOTE_E5  659
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_C6  1047
#define NOTE_D6  1175
#define SILENCIO 0

int melodia1[]        = { NOTE_E5, NOTE_E5, NOTE_E5, SILENCIO, NOTE_E5, NOTE_E5, NOTE_E5, SILENCIO, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E5, SILENCIO ,  NOTE_F5, NOTE_F5, NOTE_F5,SILENCIO , NOTE_E5, NOTE_E5, NOTE_E5,SILENCIO , NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G4,SILENCIO , NOTE_E5, NOTE_E5, NOTE_E5,SILENCIO, NOTE_E5, NOTE_E5, NOTE_E5,SILENCIO, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_D6, NOTE_E5,SILENCIO, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5,SILENCIO, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G4 };
int duracion1[]       = {   4    ,    4   ,   4    ,    4    ,    4   ,    4   ,    4   ,     4   ,    2   ,    4   ,   4    ,    4   ,   2    ,    4     ,    4    ,   4    ,    2   ,    4    ,   4   ,    4   ,    2   ,    4     ,   4   ,    4   ,    4   ,    4   ,    2    ,   4    ,   2     ,   4   ,    4   ,     2   ,    4   ,    4   ,    4   ,   2    ,   4    ,    4   ,    4   ,    4   ,    4   ,    2   ,    4   ,    4   ,    4   ,   2    ,   4    ,   4    ,   4    ,   4    ,    4   ,   4    ,    4    ,   4    ,   2    ,    4};

int tempo1 = 1000;
float factor1 = 1.50;


int melodia2[]        = { NOTE_E4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_FS4, NOTE_E4 ,SILENCIO, NOTE_D4,SILENCIO, NOTE_E4,SILENCIO,  NOTE_E4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_FS4, NOTE_E4 ,SILENCIO };
int duracion2[]       = {    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,   8    ,    8   ,    8   ,     8  ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    4    ,    8   ,    8   ,   32   ,   8    ,    8    ,     8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,   8    ,    8   ,    8   ,     8  ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8   ,    8    ,    4    ,    8    };
int tempo2 = 1400;
float factor2 = 1.50;

void melodiaCrazyFrog() {
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < sizeof(melodia2) / 2; i++) {
    int duracionNota2 = tempo2 / duracion2[i];
    tone(buzzer, melodia2[i], duracionNota2);
    int pausa2 = duracionNota2 * factor2;
    delay(pausa2);
    noTone(buzzer);
  }
}

void melodiaNavidad()
{
  pinMode(buzzer, OUTPUT);
  
  for(int i = 0 ; i < sizeof (melodia1) /2 ; i++ )
  
  {
    
  	int duracionNota1 = tempo1 / duracion1[ i ];
    
    tone (buzzer, melodia1[ i ], duracionNota1 );
  
    int pausa1 = duracionNota1 * factor1;
    delay( pausa1 );
    
    noTone( buzzer );
      
  }
}

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

  bluetooth.begin(9600);
}

void adelante() {
  analogWrite(IN1, VELOCIDAD_ALTA);
  analogWrite(IN2, 0);
  analogWrite(IN3, VELOCIDAD_ALTA);
  analogWrite(IN4, 0);
  mostrar_Carita();
}

void reversa() {
  analogWrite(IN1, 0);
  analogWrite(IN2, VELOCIDAD_ALTA);
  analogWrite(IN3, 0);
  analogWrite(IN4, VELOCIDAD_ALTA);
  mostrar_CaritaE();
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

void detener() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  mostrar_CaritaC();
}

void loop() {
  if (bluetooth.available()) {
    char comando = bluetooth.read();
    switch (comando) {
      case 'A':
        adelante();
        break;
      case 'R':
        reversa();
        break;
      case 'D':
        detener();
        break;
      case 'I':
        izquierda();
        break;
      case 'L':
        derecha();
        break;
    }
  }
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
}
