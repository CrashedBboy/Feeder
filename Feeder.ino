#include <EtherCard.h>
#include <stdio.h>
#include <Servo.h>
byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

static byte session;

byte Ethernet::buffer[700];
Stash stash;

static void reqSend () {
  Serial.println("Starting connection...");

  Stash::prepare(PSTR("Hello I'm Feeder"));

  session = ether.tcpSend();
}
Servo myservo;
const int servoPin = 5;  // Servo (SG 90)
const int tip120pin = 6; // transistor (TIP 120)

void setup() {
  Serial.begin(9600);
  pinMode(tip120pin, OUTPUT);
  analogWrite(tip120pin, 0); 
  myservo.attach(servoPin);
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
    
  ether.hisip[0] = 140;
  ether.hisip[1] = 115;
  ether.hisip[2] = 189;
  ether.hisip[3] = 142;
  ether.hisport = 3001;
  ether.persistTcpConnection(true);
  reqSend();
  //feed(1); 
}

void loop() {
  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println("Got a response!");
    Serial.println(reply);
    if(reply[0] == 'f') {
      Serial.println("Got it!");
      feed(1);
    }
  }
}

/* Control Servo */
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
