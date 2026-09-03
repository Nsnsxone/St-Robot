#include <WiFi.h>
#include <WebServer.h>
// รถตัวที่ ..
const int CAR_ID = 1; 

// เปลี่ยนตรงนี้ อ้างอิงจาก ipconfig
IPAddress local_IP(192, 168, 137, 49 + CAR_ID);  //ip host
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);

const int motorLeft_1  = 26; 
const int motorLeft_2  = 25; 
const int motorRight_1 = 32; 
const int motorRight_2 = 33; 

const int pwmFreq       = 5000;   // 5 kHz
const int pwmResolution = 8;      // 0-255

const int motorSpeed =  225; 
const int motorSpeed_t = 25; 
// เปลี่ยนตรงนี้ รหัสไวไฟ
const char* ssid     = "st-sci";      
const char* password = "12345678";

WebServer server(80);

void moveForward() {
  ledcWrite(motorLeft_1, 0);ledcWrite(motorRight_2, 0);          
  ledcWrite(motorRight_1, motorSpeed); ledcWrite(motorLeft_2, motorSpeed);
}

void moveBackward() {
  ledcWrite(motorLeft_1, motorSpeed);  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, 0);          ledcWrite(motorRight_2, motorSpeed);
}

void turnLeft() {
  ledcWrite(motorLeft_1, motorSpeed_t);           ledcWrite(motorLeft_2, -50);
  ledcWrite(motorRight_1, motorSpeed_t);          ledcWrite(motorRight_2, -50);
}

void turnRight() {
  ledcWrite(motorLeft_1, -50);  ledcWrite(motorLeft_2, motorSpeed_t);
  ledcWrite(motorRight_1, -50); ledcWrite(motorRight_2, motorSpeed_t);
}

void moveStop() {
  ledcWrite(motorLeft_1, 0);  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, 0); ledcWrite(motorRight_2, 0);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang=\"th\" translate=\"no\"><head><meta charset=\"utf-8\">";
  html += "<meta name=\"google\" content=\"notranslate\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover\">";
  
  html += "<style>";
  html += "* { box-sizing: border-box; touch-action: manipulation; -webkit-touch-callout: none; -webkit-user-select: none; user-select: none; }";
  html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; text-align: center; background-color: #0b0e14; color: white; margin: 0; padding: 10px; height: 100vh; display: flex; flex-direction: column; justify-content: flex-start; align-items: center; overflow: hidden; }";
  html += "header { width: 100%; max-width: 400px; display: flex; justify-content: space-between; align-items: center; margin-top: 15px; margin-bottom: 30px; padding: 0 10px; }";
  html += "h1 { margin: 0; font-size: 20px; color: #00A3FF; font-weight: 700; letter-spacing: -0.5px; }";
  html += ".car-badge { background: linear-gradient(135deg, #FF9900, #FF5500); color: #fff; padding: 4px 14px; border-radius: 20px; font-size: 12px; font-weight: 800; box-shadow: 0 4px 12px rgba(255,102,0,0.3); }";
  
  html += ".d-pad { display: grid; grid-template-columns: repeat(3, 95px); grid-template-rows: repeat(3, 95px); gap: 12px; justify-content: center; align-items: center; }";
  html += ".btn { width: 100%; height: 100%; color: white; background: rgba(255, 255, 255, 0.08); backdrop-filter: blur(10px); -webkit-backdrop-filter: blur(10px); border: 1px solid rgba(255, 255, 255, 0.15); border-radius: 24px; cursor: pointer; outline: none; display: flex; flex-direction: column; justify-content: center; align-items: center; box-shadow: 0 8px 32px rgba(0, 0, 0, 0.37); -webkit-tap-highlight-color: transparent; }";
  html += ".btn:active { background: #00A3FF; border-color: #00E0FF; box-shadow: 0 0 20px rgba(0, 163, 255, 0.6); }";
  
  html += ".btn-icon { font-size: 26px; line-height: 1; margin-bottom: 4px; }";
  html += ".btn-text { font-size: 11px; color: rgba(255, 255, 255, 0.8); font-weight: 600; }";
  html += "</style></head><body>";
  
  html += "<header><h1>ESP32 Remote</h1><div class='car-badge'>คันที่: " + String(CAR_ID) + "</div></header>";
  
  html += "<div class='d-pad'>";
  
  html += "<div></div>";
  html += "<button class='btn' ontouchstart=\"handlePress(event, '/forward')\" ontouchend=\"handleRelease(event)\" onmousedown=\"handlePress(event, '/forward')\" onmouseup=\"handleRelease(event)\"><span class='btn-icon'>&#x2B06;</span><span class='btn-text'>เดินหน้า</span></button>";
  html += "<div></div>";
  
  html += "<button class='btn' ontouchstart=\"handlePress(event, '/left')\" ontouchend=\"handleRelease(event)\" onmousedown=\"handlePress(event, '/left')\" onmouseup=\"handleRelease(event)\"><span class='btn-icon'>&#x2B05;</span><span class='btn-text'>เลี้ยวซ้าย</span></button>";
  html += "<div></div>";
  html += "<button class='btn' ontouchstart=\"handlePress(event, '/right')\" ontouchend=\"handleRelease(event)\" onmousedown=\"handlePress(event, '/right')\" onmouseup=\"handleRelease(event)\"><span class='btn-icon'>&#x27A1;</span><span class='btn-text'>เลี้ยวขวา</span></button>";
  
  html += "<div></div>";
  html += "<button class='btn' ontouchstart=\"handlePress(event, '/backward')\" ontouchend=\"handleRelease(event)\" onmousedown=\"handlePress(event, '/backward')\" onmouseup=\"handleRelease(event)\"><span class='btn-icon'>&#x2B07;</span><span class='btn-text'>ถอยหลัง</span></button>";
  html += "<div></div>";
  
  html += "</div>"; 
  
  html += "<script>";
  html += "let activeController = null;";
  html += "function sendCmd(url) { if(activeController) activeController.abort(); activeController = new AbortController(); fetch(url, { signal: activeController.signal }).catch(err => {}); }";
  html += "function handlePress(e, url) { if(e.cancelable) e.preventDefault(); sendCmd(url); }";
  html += "function handleRelease(e) { if(e.cancelable) e.preventDefault(); sendCmd('/stop'); }";
  html += "</script></body></html>";




  // extra func 
  html += "<style>";
  html += "* { box-sizing: border-box; touch-action: manipulation; -webkit-touch-callout: none; -webkit-user-select: none; user-select: none; }";
  html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; text-align: center; background-color: #0b0e14; color: white; margin: 0; padding: 10px; height: 100vh; display: flex; flex-direction: column; justify-content: flex-start; align-items: center; overflow: hidden; }";
  html += "header { width: 100%; max-width: 400px; display: flex; justify-content: space-between; align-items: center; margin-top: 15px; margin-bottom: 30px; padding: 0 10px; }";
  html += "h1 { margin: 0; font-size: 20px; color: #00A3FF; font-weight: 700; letter-spacing: -0.5px; }";
  html += ".car-badge { background: linear-gradient(135deg, #FF9900, #FF5500); color: #fff; padding: 4px 14px; border-radius: 20px; font-size: 12px; font-weight: 800; box-shadow: 0 4px 12px rgba(255,102,0,0.3); }";

  html += ".start-btn { width: 120px; height: 60px; margin-bottom: 20px; }";

  html += ".d-pad { display: grid; grid-template-columns: repeat(3, 95px); grid-template-rows: repeat(3, 95px); gap: 12px; justify-content: center; align-items: center; }";
  html += ".btn { width: 100%; height: 100%; color: white; background: rgba(255, 255, 255, 0.08); backdrop-filter: blur(10px); -webkit-backdrop-filter: blur(10px); border: 1px solid rgba(255, 255, 255, 0.15); border-radius: 24px; cursor: pointer; outline: none; display: flex; flex-direction: column; justify-content: center; align-items: center; box-shadow: 0 8px 32px rgba(0, 0, 0, 0.37); -webkit-tap-highlight-color: transparent; }";
  html += ".btn:active { background: #00A3FF; border-color: #00E0FF; box-shadow: 0 0 20px rgba(0, 163, 255, 0.6); }";
  html += ".btn-icon { font-size: 26px; line-height: 1; margin-bottom: 4px; }";
  html += ".btn-text { font-size: 11px; color: rgba(255, 255, 255, 0.8); font-weight: 600; }";
  html += "</style></head><body>";

  //html += "<header><h1>ESP32 Remote</h1><div class='car-badge'>คันที่: " + String(CAR_ID) + "</div></header>";

  // Start button
  html += "<button class='btn start-btn' onclick=\"sendCmd('/start')\">";
  html += "<span class='btn-icon'>&#x25B6;</span>";
  html += "<span class='btn-text'>Auto Mode ; handleSt</span>";
  html += "</button>";

  html += "<div class='d-pad'>";





  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // ตั้งค่า PWM ให้กับขามอเตอร์แต่ละขา (ESP32 Core 3.x API ใหม่)
  ledcAttach(motorLeft_1,  pwmFreq, pwmResolution);
  ledcAttach(motorLeft_2,  pwmFreq, pwmResolution);
  ledcAttach(motorRight_1, pwmFreq, pwmResolution);
  ledcAttach(motorRight_2, pwmFreq, pwmResolution);
  
  moveStop(); 

  WiFi.setSleep(false);
  WiFi.mode(WIFI_STA);
  
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("[ERROR] Static IP Configuration Failed!");
  }

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to WiFi SSID: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print(".");
  }

  Serial.println("\n--- WiFi Connected ---");
  Serial.print("Car ID: "); Serial.println(CAR_ID);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/forward", []() { moveForward(); Serial.println("[Cmd] FORWARD"); server.send(200, "text/plain", "Forward"); });
  server.on("/backward", []() { moveBackward(); Serial.println("[Cmd] BACKWARD"); server.send(200, "text/plain", "Backward"); });
  server.on("/left", []() { turnLeft(); Serial.println("[Cmd] LEFT"); server.send(200, "text/plain", "Left"); });
  server.on("/right", []() { turnRight(); Serial.println("[Cmd] RIGHT"); server.send(200, "text/plain", "Right"); });
  server.on("/stop", []() { moveStop(); Serial.println("[Cmd] STOP"); server.send(200, "text/plain", "Stop"); });


  server.on("/start", handleStart);
  

  server.begin();
  Serial.println("HTTP WebServer Started.");
  

}

void handleStart() {
  // func auto
  moveForward();
  delay(3000);
  
  
  turnRight();
  delay(2000);
  
  
  moveForward();
  delay(2500);
  
  turnRight();
  delay(750);

  turnLeft();
  delay(1250);

  moveForward();
  delay(2500);

  moveStop();
}

void loop() {
  server.handleClient();
}
