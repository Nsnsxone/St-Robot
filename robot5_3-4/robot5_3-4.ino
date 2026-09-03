#include <WiFi.h>
#include <WebServer.h>

const int motorLeft_1  = 26; 
const int motorLeft_2  = 25; 
const int motorRight_1 = 32; 
const int motorRight_2 = 33; 

const int pwmFreq       = 5000;   // 5 kHz
const int pwmResolution = 8;      // 0-255

const int motorSpeed = 240; 

const int A = 3.3;
const int B = 4;
WebServer server(80);

void w() {
  ledcWrite(motorLeft_1, 0);           
  ledcWrite(motorLeft_2, motorSpeed - A);
  ledcWrite(motorRight_1, motorSpeed + B ); 
  ledcWrite(motorRight_2, 0);
}

void s() {
  ledcWrite(motorLeft_1, motorSpeed - A);  
  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, 0);          
  ledcWrite(motorRight_2, motorSpeed + B);
}

void a() {
  ledcWrite(motorLeft_1, 0);           
  ledcWrite(motorLeft_2, motorSpeed-50+A);
  ledcWrite(motorRight_1, 0);          
  ledcWrite(motorRight_2, motorSpeed-50-B);
}

void d() {
  ledcWrite(motorLeft_1, motorSpeed-50+A);  
  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, motorSpeed-50-B); 
  ledcWrite(motorRight_2, 0);
}

void Stop() {
  ledcWrite(motorLeft_1, 0);  
  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, 0); 
  ledcWrite(motorRight_2, 0);
}


void setup() {


  Serial.begin(115200);
  
  // ตั้งค่า PWM ให้กับขามอเตอร์แต่ละขา (ESP32 Core 3.x API ใหม่)
  ledcAttach(motorLeft_1,  pwmFreq, pwmResolution);
  ledcAttach(motorLeft_2,  pwmFreq, pwmResolution);
  ledcAttach(motorRight_1, pwmFreq, pwmResolution);
  ledcAttach(motorRight_2, pwmFreq, pwmResolution);
  Stop(); 
  w();
  delay(2000);
  Stop(); 
  delay(300);
  d();
  delay(900);
  Stop();
  delay(300);  
  w();
  delay(1700);
  Stop(); 
  delay(300);
  d();
  delay(900);
  Stop();  
  delay(300);
  w();
  delay(2000);
  Stop(); 
  delay(300);
  a();
  delay(900);
  Stop();  

  //server.on("/", handleRoot);
  //server.on("/forward", []() { moveForward(); Serial.println("[Cmd] FORWARD"); server.send(200, "text/plain", "Forward"); });
  //server.on("/backward", []() { moveBackward(); Serial.println("[Cmd] BACKWARD"); server.send(200, "text/plain", "Backward"); });
  //server.on("/left", []() { turnLeft(); Serial.println("[Cmd] LEFT"); server.send(200, "text/plain", "Left"); });
  //server.on("/right", []() { turnRight(); Serial.println("[Cmd] RIGHT"); server.send(200, "text/plain", "Right"); });
  //server.on("/stop", []() { moveStop(); Serial.println("[Cmd] STOP"); server.send(200, "text/plain", "Stop"); });

}

void loop() {
  //server.handleClient();
}