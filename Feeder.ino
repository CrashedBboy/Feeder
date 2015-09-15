#include <stdio.h>
#include <Servo.h>
#include "DS1302.h"

Servo myservo;
const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock
const int servoPin = 9;  // Servo
const int tip120pin = 8; // transistor
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void setup() {
  Serial.begin(9600);
  pinMode(tip120pin, OUTPUT);
  analogWrite(tip120pin, 0); 
  myservo.attach(servoPin);
}

void loop() {
  Time counter = rtc.time();
  if (counter.sec == 0 && counter.min == 0 ) {
    feed(1);  
  }
  delay(1000);
}

/* Control modules */
/*-----------------------------------------------------------------------*/
void feed(int amount){
  analogWrite(tip120pin, 255);
  for (int i = 0; i < amount; i++) {
    myservo.write(180);
    delay(1500);
    myservo.write(0);
    delay(1500);
  }
  analogWrite(tip120pin, 0);
}

/* Time and Date */
/*-----------------------------------------------------------------------*/
void printTime() {
  Time t = rtc.time();
  char buf[24];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  Serial.println(buf);
}

void setTime(int year,int month,int day,int hour,int min,int sec, Time::Day weekday) {  //setTime(2014, 9, 13, 13, 55, 00, Time::kSunday);
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(year, month, day, hour, min, sec, weekday);
  rtc.time(t);
}
