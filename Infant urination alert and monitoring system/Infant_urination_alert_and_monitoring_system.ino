#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SENSOR_PIN A0
#define BUZZER_PIN D5
#define LED_PIN D6

const char* ssid = "DiaperMonitor";
const char* password = "12345678";

ESP8266WebServer server(80);

int threshold = 600;
int sensorValue = 0;
int moisturePercent = 0;
String statusText = "DRY";

void handleRoot()
{
  String html = R"====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta http-equiv="refresh" content="2">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Smart Diaper Monitor</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
}

body{
font-family:Segoe UI,sans-serif;
background:linear-gradient(135deg,#667eea,#764ba2);
min-height:100vh;
display:flex;
justify-content:center;
align-items:center;
padding:20px;
}

.card{
background:white;
width:100%;
max-width:550px;
padding:30px;
border-radius:20px;
box-shadow:0 20px 40px rgba(0,0,0,0.25);
text-align:center;
}

.title{
font-size:32px;
font-weight:bold;
color:#333;
margin-bottom:10px;
}

.subtitle{
color:#666;
margin-bottom:25px;
}

.status{
font-size:36px;
font-weight:bold;
padding:15px;
border-radius:15px;
margin-bottom:20px;
}

.wet{
background:#ffe5e5;
color:#ff0000;
animation:pulse 1s infinite;
}

.dry{
background:#e8ffe8;
color:#00aa00;
}

@keyframes pulse{
0%{transform:scale(1);}
50%{transform:scale(1.05);}
100%{transform:scale(1);}
}

.bar{
width:100%;
height:30px;
background:#ddd;
border-radius:30px;
overflow:hidden;
margin-top:20px;
}

.fill{
height:100%;
background:linear-gradient(90deg,#00c853,#64dd17);
line-height:30px;
font-weight:bold;
color:white;
}

.sensorBox{
display:grid;
grid-template-columns:1fr 1fr;
gap:15px;
margin-top:25px;
}

.info{
background:#f7f7f7;
padding:15px;
border-radius:12px;
}

.label{
font-size:14px;
color:#666;
}

.value{
font-size:28px;
font-weight:bold;
margin-top:5px;
}

.clock{
font-size:20px;
font-weight:bold;
color:#667eea;
margin-top:20px;
}

.footer{
margin-top:20px;
font-size:14px;
color:#777;
}

.alert{
margin-top:15px;
background:#ff4444;
color:white;
padding:12px;
border-radius:10px;
font-weight:bold;
}

</style>
</head>

<body>

<div class="card">

<div class="title">
BABY DIAPER MONITOR
</div>

<div class="subtitle">
Real Time Moisture Detection System
</div>
)====";

  html += "<div class='status ";
  html += (statusText == "WET") ? "wet" : "dry";
  html += "'>";
  html += statusText;
  html += "</div>";

  if(statusText == "WET")
  {
    html += "<div class='alert'>CHANGE DIAPER IMMEDIATELY</div>";
  }

  html += "<h3 style='margin-top:20px;'>Moisture Level</h3>";

  html += "<div class='bar'><div class='fill' style='width:";
  html += moisturePercent;
  html += "%'>";
  html += moisturePercent;
  html += "%</div></div>";

  html += "<div class='sensorBox'>";

  html += "<div class='info'>";
  html += "<div class='label'>Moisture</div>";
  html += "<div class='value'>";
  html += moisturePercent;
  html += "%</div></div>";

  html += "<div class='info'>";
  html += "<div class='label'>Status</div>";
  html += "<div class='value'>";
  html += statusText;
  html += "</div></div>";

  html += "</div>";

  html += R"====(

<div class="clock" id="clock"></div>

<div class="footer">
Smart Diaper Monitoring System
</div>

</div>

<script>

function updateClock(){
document.getElementById("clock").innerHTML =
new Date().toLocaleTimeString();
}

setInterval(updateClock,1000);
updateClock();

</script>

</body>
</html>
)====";

  server.send(200, "text/html", html);
}

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("AP Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop()
{
  server.handleClient();

  sensorValue = analogRead(SENSOR_PIN);

  moisturePercent = map(sensorValue, 1023, 0, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  if(sensorValue < threshold)
  {
    statusText = "WET";

    tone(BUZZER_PIN, 1500);

    digitalWrite(LED_PIN, LOW);

    Serial.println("ALERT: Diaper is Wet!");
  }
  else
  {
    statusText = "DRY";

    noTone(BUZZER_PIN);

    digitalWrite(LED_PIN, HIGH);

    Serial.println("Diaper is Dry");
  }

  delay(500);
}
