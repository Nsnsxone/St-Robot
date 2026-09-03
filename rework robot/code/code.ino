#include <WiFi.h>
#include <WebServer.h>

const int CAR_ID = 1;

IPAddress local_IP(192, 168, 137, 49 + CAR_ID);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);

// =========================
// Motor Pins
// =========================
const int motorLeft_1  = 26;
const int motorLeft_2  = 25;
const int motorRight_1 = 32;
const int motorRight_2 = 33;

// =========================
// Default PWM Settings
// =========================
const int defaultPwmFreq       = 5000;
const int defaultPwmResolution = 8;

// =========================
// Default Motor Speed
// =========================
const int defaultMotorSpeed   = 240;
const int defaultMotorSpeed_t = 25;

// =========================
// Runtime Settings
// =========================
int pwmFreq       = defaultPwmFreq;
int pwmResolution = defaultPwmResolution;

int motorSpeed   = defaultMotorSpeed;
int motorSpeed_t = defaultMotorSpeed_t;

// =========================
// WiFi
// =========================
const char* ssid     = "DESKTOPVK0NUK8";
const char* password = "k1234567";

WebServer server(80);


// =====================================================
// Motor Functions
// =====================================================

void moveForward() {
  ledcWrite(motorLeft_1, 0);
  ledcWrite(motorRight_2, 0);

  ledcWrite(motorRight_1, motorSpeed);
  ledcWrite(motorLeft_2, motorSpeed);
}

void moveBackward() {
  ledcWrite(motorLeft_1, motorSpeed);
  ledcWrite(motorLeft_2, 0);

  ledcWrite(motorRight_1, 0);
  ledcWrite(motorRight_2, motorSpeed);
}

void turnLeft() {
  ledcWrite(motorLeft_1, motorSpeed_t);
  ledcWrite(motorLeft_2, 0);

  ledcWrite(motorRight_1, motorSpeed_t);
  ledcWrite(motorRight_2, 0);
}

void turnRight() {
  ledcWrite(motorLeft_1, 0);
  ledcWrite(motorLeft_2, motorSpeed_t);

  ledcWrite(motorRight_1, 0);
  ledcWrite(motorRight_2, motorSpeed_t);
}

void moveStop() {
  ledcWrite(motorLeft_1, 0);
  ledcWrite(motorLeft_2, 0);
  ledcWrite(motorRight_1, 0);
  ledcWrite(motorRight_2, 0);
}


// =====================================================
// Start Function
// =====================================================

void handleStart() {

  Serial.println();
  Serial.println("========== START ==========");
  Serial.print("Motor Speed : ");
  Serial.println(motorSpeed);

  Serial.print("Turn Speed  : ");
  Serial.println(motorSpeed_t);

  Serial.print("PWM Freq    : ");
  Serial.println(pwmFreq);

  Serial.print("PWM Res     : ");
  Serial.println(pwmResolution);

  Serial.println("===========================");


  
  moveStop();

  moveForward();
  delay(2000);

  moveStop();
  delay(300);

  turnRight();
  delay(900);

  moveStop();
  delay(300);

  moveForward();
  delay(1700);

  moveStop();
  delay(300);

  turnRight();
  delay(900);

  moveStop();
  delay(300);

  moveForward();
  delay(2000);

  moveStop();
  delay(300);

  turnLeft();
  delay(900);

  moveStop();


  Serial.println("[Start] Sequence Finished.");
}


// =====================================================
// Web UI
// =====================================================

void handleRoot() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en" translate="no">

<head>

<meta charset="utf-8">

<meta name="google" content="notranslate">

<meta name="viewport"
content="width=device-width,
initial-scale=1.0,
maximum-scale=1.0,
user-scalable=no,
viewport-fit=cover">

<title>ESP32 Motor Settings</title>

<style>

* {
  box-sizing: border-box;
  -webkit-tap-highlight-color: transparent;
}

body {

  font-family:
    -apple-system,
    BlinkMacSystemFont,
    "Segoe UI",
    Roboto,
    Helvetica,
    Arial,
    sans-serif;

  background:
    radial-gradient(
      circle at top,
      #172033 0%,
      #0b0e14 55%,
      #07090d 100%
    );

  color: white;

  margin: 0;

  min-height: 100vh;

  padding:
    25px
    16px
    30px;

  display: flex;

  justify-content: flex-start;

  align-items: center;

  flex-direction: column;
}


/* =========================
   Header
========================= */

.header {

  width: 100%;

  max-width: 430px;

  display: flex;

  justify-content: space-between;

  align-items: center;

  margin-bottom: 25px;

}

.title {

  font-size: 22px;

  font-weight: 700;

  color: #00a3ff;

  letter-spacing: -0.5px;

}

.car {

  background:
    linear-gradient(
      135deg,
      #ff9900,
      #ff5500
    );

  padding:
    6px
    14px;

  border-radius: 20px;

  font-size: 12px;

  font-weight: 800;

  box-shadow:
    0 4px 15px
    rgba(255, 102, 0, 0.3);

}


/* =========================
   Main Card
========================= */

.card {

  width: 100%;

  max-width: 430px;

  background:
    rgba(255,255,255,0.06);

  border:
    1px solid
    rgba(255,255,255,0.12);

  border-radius: 28px;

  padding: 22px;

  box-shadow:
    0 20px 60px
    rgba(0,0,0,0.35);

  backdrop-filter:
    blur(15px);

  -webkit-backdrop-filter:
    blur(15px);

}


/* =========================
   Section
========================= */

.section-title {

  font-size: 13px;

  color:
    rgba(255,255,255,0.55);

  font-weight: 700;

  text-transform:
    uppercase;

  letter-spacing: 1px;

  margin-bottom: 18px;

}


/* =========================
   Input
========================= */

.setting {

  margin-bottom: 18px;

}

.setting:last-child {

  margin-bottom: 0;

}

.label-row {

  display: flex;

  justify-content:
    space-between;

  align-items: center;

  margin-bottom: 8px;

}

.label {

  font-size: 14px;

  font-weight: 600;

}

.unit {

  font-size: 11px;

  color:
    rgba(255,255,255,0.4);

}


input {

  width: 100%;

  height: 54px;

  border-radius: 15px;

  border:
    1px solid
    rgba(255,255,255,0.15);

  background:
    rgba(0,0,0,0.25);

  color: white;

  font-size: 20px;

  font-weight: 700;

  text-align: center;

  outline: none;

  padding: 0 15px;

  transition: 0.2s;

}

input:focus {

  border-color:
    #00a3ff;

  box-shadow:
    0 0 0 3px
    rgba(0,163,255,0.12);

}


/* =========================
   Start Button
========================= */

.start {

  width: 100%;

  height: 65px;

  margin-top: 25px;

  border: none;

  border-radius: 18px;

  background:
    linear-gradient(
      135deg,
      #00a3ff,
      #0077ff
    );

  color: white;

  font-size: 18px;

  font-weight: 800;

  letter-spacing: 0.3px;

  cursor: pointer;

  box-shadow:
    0 10px 30px
    rgba(0,163,255,0.28);

  transition:
    transform 0.1s,
    box-shadow 0.1s;

}

.start:active {

  transform:
    scale(0.97);

  box-shadow:
    0 4px 15px
    rgba(0,163,255,0.25);

}


/* =========================
   Status
========================= */

.status {

  text-align: center;

  margin-top: 16px;

  font-size: 12px;

  color:
    rgba(255,255,255,0.45);

}

.status.ok {

  color: #35e58a;

}

</style>

</head>


<body>


<div class="header">

  <div class="title">
    ESP32 Motor
  </div>

  <div class="car">
    CAR )rawliteral";

  html += String(CAR_ID);

  html += R"rawliteral(
  </div>

</div>


<div class="card">


  <div class="section-title">
    Motor Settings
  </div>


  <!-- Motor Speed -->

  <div class="setting">

    <div class="label-row">

      <div class="label">
        Motor Speed
      </div>

      <div class="unit">
        0 - 255
      </div>

    </div>

    <input
      id="motorSpeed"
      type="number"
      min="0"
      max="255"
      value=")rawliteral";

  html += String(motorSpeed);

  html += R"rawliteral("
    >

  </div>


  <!-- Turn Speed -->

  <div class="setting">

    <div class="label-row">

      <div class="label">
        Turn Speed
      </div>

      <div class="unit">
        0 - 255
      </div>

    </div>

    <input
      id="turnSpeed"
      type="number"
      min="0"
      max="255"
      value=")rawliteral";

  html += String(motorSpeed_t);

  html += R"rawliteral("
    >

  </div>


  <!-- PWM Frequency -->

  <div class="setting">

    <div class="label-row">

      <div class="label">
        PWM Frequency
      </div>

      <div class="unit">
        Hz
      </div>

    </div>

    <input
      id="pwmFreq"
      type="number"
      min="1"
      max="100000"
      value=")rawliteral";

  html += String(pwmFreq);

  html += R"rawliteral("
    >

  </div>


  <!-- PWM Resolution -->

  <div class="setting">

    <div class="label-row">

      <div class="label">
        PWM Resolution
      </div>

      <div class="unit">
        bits
      </div>

    </div>

    <input
      id="pwmResolution"
      type="number"
      min="1"
      max="16"
      value=")rawliteral";

  html += String(pwmResolution);

  html += R"rawliteral("
    >

  </div>


  <button
    class="start"
    onclick="startCar()">

    ▶ START

  </button>


  <div
    id="status"
    class="status">

    Ready

  </div>


</div>


<script>


function startCar() {

  const motor =
    document.getElementById(
      "motorSpeed"
    ).value;

  const turn =
    document.getElementById(
      "turnSpeed"
    ).value;

  const freq =
    document.getElementById(
      "pwmFreq"
    ).value;

  const resolution =
    document.getElementById(
      "pwmResolution"
    ).value;


  const status =
    document.getElementById(
      "status"
    );


  // =========================
  // Validate
  // =========================

  if (
    motor < 0 ||
    motor > 255 ||
    turn < 0 ||
    turn > 255 ||
    freq < 1 ||
    resolution < 1 ||
    resolution > 16
  ) {

    status.innerText =
      "Invalid value";

    status.className =
      "status";

    return;

  }


  status.innerText =
    "Sending settings...";

  status.className =
    "status";


  const url =
    "/start?motor=" +
    encodeURIComponent(motor) +

    "&turn=" +
    encodeURIComponent(turn) +

    "&freq=" +
    encodeURIComponent(freq) +

    "&resolution=" +
    encodeURIComponent(resolution);


  fetch(url)

    .then(response => {

      if (!response.ok) {
        throw new Error();
      }

      return response.text();

    })

    .then(data => {

      status.innerText =
        "Started";

      status.className =
        "status ok";

    })

    .catch(error => {

      status.innerText =
        "Connection Error";

      status.className =
        "status";

    });

}

</script>


</body>

</html>

)rawliteral";


  server.send(
    200,
    "text/html",
    html
  );
}


// =====================================================
// Handle Start + Receive Settings
// =====================================================

void handleStartRequest() {

  // รับค่าจาก Web UI

  if (server.hasArg("motor")) {
    motorSpeed =
      server.arg("motor").toInt();
  }

  if (server.hasArg("turn")) {
    motorSpeed_t =
      server.arg("turn").toInt();
  }

  if (server.hasArg("freq")) {
    pwmFreq =
      server.arg("freq").toInt();
  }

  if (server.hasArg("resolution")) {
    pwmResolution =
      server.arg("resolution").toInt();
  }


  // =========================
  // จำกัดค่าป้องกันผิดพลาด
  // =========================

  motorSpeed =
    constrain(
      motorSpeed,
      0,
      255
    );

  motorSpeed_t =
    constrain(
      motorSpeed_t,
      0,
      255
    );

  pwmFreq =
    constrain(
      pwmFreq,
      1,
      100000
    );

  pwmResolution =
    constrain(
      pwmResolution,
      1,
      16
    );


  // =========================
  // ตอบกลับ Browser ก่อน
  // =========================

  server.send(
    200,
    "text/plain",
    "START"
  );


  // =========================
  // แสดงค่าที่รับมา
  // =========================

  Serial.println();
  Serial.println("========== SETTINGS ==========");

  Serial.print("Motor Speed   : ");
  Serial.println(motorSpeed);

  Serial.print("Turn Speed    : ");
  Serial.println(motorSpeed_t);

  Serial.print("PWM Frequency : ");
  Serial.println(pwmFreq);

  Serial.print("PWM Resolution: ");
  Serial.println(pwmResolution);

  Serial.println("==============================");


  // =========================
  // Start sequence
  // =========================

  handleStart();
}


// =====================================================
// Setup
// =====================================================

void setup() {

  Serial.begin(115200);


  // =========================
  // PWM
  // =========================

  ledcAttach(
    motorLeft_1,
    pwmFreq,
    pwmResolution
  );

  ledcAttach(
    motorLeft_2,
    pwmFreq,
    pwmResolution
  );

  ledcAttach(
    motorRight_1,
    pwmFreq,
    pwmResolution
  );

  ledcAttach(
    motorRight_2,
    pwmFreq,
    pwmResolution
  );


  moveStop();


  // =========================
  // WiFi
  // =========================

  WiFi.setSleep(false);

  WiFi.mode(WIFI_STA);


  if (!WiFi.config(
        local_IP,
        gateway,
        subnet
      )) {

    Serial.println(
      "[ERROR] Static IP Configuration Failed!"
    );

  }


  WiFi.begin(
    ssid,
    password
  );


  Serial.println();

  Serial.print(
    "Connecting to WiFi SSID: "
  );

  Serial.println(ssid);


  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");

  }


  Serial.println(
    "\n--- WiFi Connected ---"
  );

  Serial.print(
    "Car ID: "
  );

  Serial.println(CAR_ID);


  Serial.print(
    "IP Address: "
  );

  Serial.println(
    WiFi.localIP()
  );


  // =========================
  // Web Server
  // =========================

  server.on(
    "/",
    handleRoot
  );

  server.on(
    "/start",
    handleStartRequest
  );


  server.begin();


  Serial.println(
    "HTTP WebServer Started."
  );

}


// =====================================================
// Loop
// =====================================================

void loop() {

  server.handleClient();

}