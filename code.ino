#include <AFMotor.h>

// Mecanum: FL=M1, BL=M2, FR=M3, BR=M4
AF_DCMotor m1(1), m2(2), m3(3), m4(4);

String cmd;
uint8_t spd = 150;

void setup() {
  Serial.begin(9600);
  setAllSpeed(spd);
  allStop();
}

void loop() {
  readCommand();
  if (cmd.length() == 0) return;

  String s = cmd; s.trim(); s.toUpperCase();

  if (s == "FORWARD")       runAll(FORWARD);
  else if (s == "BACKWARD") runAll(BACKWARD);

  // Strafe
  else if (s == "RIGHT")  {  // strafe left
    m1.run(BACKWARD); m2.run(FORWARD);  m3.run(BACKWARD); m4.run(FORWARD);
  }
  else if (s == "LEFT") {  // strafe right
    m1.run(FORWARD);  m2.run(BACKWARD); m3.run(FORWARD);  m4.run(BACKWARD);
  }

  // Diagonal
  else if (s == "FORWARDLEFT") { // FL & BR forward
    m1.run(FORWARD); m2.run(RELEASE); m3.run(RELEASE); m4.run(FORWARD);
  }
  else if (s == "FORWARDRIGHT") { // FR & BL forward
    m1.run(RELEASE); m2.run(FORWARD); m3.run(FORWARD); m4.run(RELEASE);
  }
  else if (s == "BACKWARDLEFT") { // FL & BR backward
    m1.run(BACKWARD); m2.run(RELEASE); m3.run(RELEASE); m4.run(BACKWARD);
  }
  else if (s == "BACKWARDRIGHT") { // FR & BL backward
    m1.run(RELEASE); m2.run(BACKWARD); m3.run(BACKWARD); m4.run(RELEASE);
  }

  // Rotate

  else if (s == "ROTATELEFT")  { // xoay trái
    m1.run(FORWARD);
    m2.run(BACKWARD);
    m3.run(BACKWARD);
    m4.run(FORWARD);
  }
  else if (s == "ROTATERIGHT") { // xoay phải
    m1.run(BACKWARD);
    m2.run(FORWARD);
    m3.run(FORWARD);
    m4.run(BACKWARD);
  }

  else if (s == "STOP") allStop();
  else if (s == "TEST") testSequence(1500);
  else if (s.startsWith("SPEED:")) setSpeedFrom(s); // ví dụ SPEED:150

  cmd = "";
}

// ==== helpers ====
void readCommand() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c!='\n' && c!='\r') cmd += c;
    delay(2);
  }
}
void runAll(uint8_t dir){ m1.run(dir); m2.run(dir); m3.run(dir); m4.run(dir); }
void allStop(){ m1.run(RELEASE); m2.run(RELEASE); m3.run(RELEASE); m4.run(RELEASE); }
void setAllSpeed(uint8_t s){ m1.setSpeed(s); m2.setSpeed(s); m3.setSpeed(s); m4.setSpeed(s); }

void setSpeedFrom(const String &s){
  int v = s.substring(s.indexOf(':')+1).toInt();
  if (v<0) v=0; if (v>255) v=255;
  spd=v; setAllSpeed(spd);
  Serial.print("SPEED="); Serial.println(spd);
}

void testOne(AF_DCMotor &m, int idx, unsigned long ms){
  Serial.print("Test M"); Serial.println(idx);
  m.run(FORWARD);  delay(ms);
  m.run(RELEASE);  delay(400);
  m.run(BACKWARD); delay(ms);
  m.run(RELEASE);  delay(600);
}

void testSequence(unsigned long ms){
  setAllSpeed(spd);
  testOne(m1,1,ms); testOne(m2,2,ms); testOne(m3,3,ms); testOne(m4,4,ms);
}
