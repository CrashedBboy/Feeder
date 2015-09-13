#include <stdio.h>
#include "DS1302.h"
#include <Servo.h>
Servo myservo;


const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock
const int servoPin = 9;  // Servo
const int tip120pin = 8;

DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}



void printTime() {
  Time t = rtc.time();
  const String day = dayAsString(t.day);

  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  Serial.println(buf);
}

void setup() {
  Serial.begin(9600);
  
  pinMode(tip120pin, OUTPUT);
  analogWrite(tip120pin, 0); 

  rtc.writeProtect(false);
  rtc.halt(false);

  //Time t(2014, 9, 13, 13, 55, 00, Time::kSunday);
  //rtc.time(t);
  myservo.attach(servoPin);
  feed();
  feed();
}

void feed(){
  analogWrite(tip120pin, 255);
  myservo.write(180);
  delay(1500);
  myservo.write(0);
  delay(3000);
  analogWrite(tip120pin, 0);
}

// Loop and print the time every second.
void loop() {
  printTime();
  delay(1000);
}
