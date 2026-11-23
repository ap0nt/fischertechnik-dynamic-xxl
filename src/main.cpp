#include <Arduino.h>
#include <Servo.h>

int h1 = 12;

int m1 = 4;
int m2 = 5;
int m3_1 = 6;
int m3_2 = 7;
int m4_1 = 8;
int m4_2 = 9;

int led1 = 10;
int led2 = 11;

int tip1 = A0;
int tip2 = A1;
int tip3 = A2;
int tip4 = A3;

Servo Servo1;
int iz1 = 2;

char loopingDirection = 'r';
char twoLineDirection = 'l';
char blockDirection = 'r';

void playSound()
{
  tone(15, 1000);
  delay(500);

  noTone(15);
  delay(500);
}

void loopingSwitch()
{
  if (loopingDirection == 'r')
  {
    digitalWrite(m3_2, HIGH);
    delay(750);
    digitalWrite(m3_2, LOW);
    loopingDirection = 'l';
  }

  else if (loopingDirection == 'l')
  {
    digitalWrite(m3_1, HIGH);
    delay(750);
    digitalWrite(m3_1, LOW);
    loopingDirection = 'r';
  }
}

void twoLineSwitch()
{
  if (twoLineDirection == 'r')
  {
    digitalWrite(m4_2, HIGH);
    delay(750);
    digitalWrite(m4_2, LOW);
    twoLineDirection = 'l';
  }

  else if (twoLineDirection == 'l')
  {
    digitalWrite(m4_1, HIGH);
    delay(750);
    digitalWrite(m4_1, LOW);
    twoLineDirection = 'r';
  }
}

void blockSwitch()
{
  if (blockDirection == 'r')
  {
    Servo1.write(45);
    blockDirection = 'l';
  }

  else if (blockDirection == 'l')
  {
    Servo1.write(0);
    blockDirection = 'r';
  }

  delay(750);
}

void setup()
{
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3_1, OUTPUT);
  pinMode(m3_2, OUTPUT);

  pinMode(h1, INPUT);

  pinMode(tip1, INPUT);
  pinMode(tip2, INPUT);
  pinMode(tip3, INPUT);
  pinMode(tip4, INPUT);

  pinMode(iz1, OUTPUT);
  Servo1.attach(iz1);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  analogWrite(m2, 45);

  // set loopingSwitch to 'r'
  digitalWrite(m3_1, HIGH);
  delay(750);
  digitalWrite(m3_1, LOW);

  loopingSwitch();

  // set twoLineDirection to 'l'
  digitalWrite(m4_1, HIGH);
  delay(750);
  digitalWrite(m4_1, LOW);

  twoLineSwitch();

  Servo1.write(0);
  delay(750);
  blockSwitch();
}

void loop()
{
  const int tip1State = digitalRead(tip1);
  const int tip2State = digitalRead(tip2);
  const int tip3State = digitalRead(tip3);
  const int tip4State = digitalRead(tip4);

  digitalWrite(m1, tip1State);

  if (tip2State == LOW)
    blockSwitch();
  if (tip3State == LOW)
    twoLineSwitch();
  if (tip4State == LOW)
    loopingSwitch();

  // playSound();

  //Serial.println(digitalRead(h1));
}
