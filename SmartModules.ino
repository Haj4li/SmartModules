#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "Smart.h"
#include "Pages.h"

/*
  Todo:
    [ ] Set and get cookies (auth)
    [ ] add buzzer for alarm
    [ ] add ultrasonic module (for parking of course)

*/


// Photocell
Module PC(A0,true);
// Valid login led
Module LEDv(D0,false);
// Not valid login led
Module LEDl(D1,false);
// SSID and password for nodemcu AP
const char* ssid = "KhaneHoshmand";
const char* password = "12345678";

// Running server on port 80
ESP8266WebServer server(80);

// Something for debuging
const int ledPin = LED_BUILTIN;
int ledState = LOW;
int pval = 10;

// Valid username and password for login
const char* validUsername = "ali";
const char* validPassword = "1234";

// Login attempts (3 Max)
short attempts = 0;

// The main page
void handleRoot() {
  String html = "<html><body>";
  html += "<h1 id='value'>Value: " + String(pval) + "</h1>";
  html += "<button onclick=\"togglePin()\">Toggle LED</button>";
  html += "<script>function togglePin(){var xhr=new XMLHttpRequest();xhr.open('GET','/toggle',true);xhr.send();}</script>";
  html += "<script>setInterval(updateValue, 1000);function updateValue(){var xhr=new XMLHttpRequest();xhr.onreadystatechange=function(){if(xhr.readyState===4&&xhr.status===200){document.getElementById('value').innerHTML='Value: '+xhr.responseText;}};xhr.open('GET','/getvalue',true);xhr.send();}</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleToggle() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  server.send(200, "text/plain", "LED toggled");
}

void handleLogin_Page()
{
  server.send(200, "text/html", p_login);
}

// Get module value everytime function called
void handleGetValue() {
  pval = PC.GetValue<int>(Analog);
  Serial.println(pval);
  server.send(200, "text/plain", String(pval));
}
// validate posted args for login user
void validateLogin()
{
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == validUsername && server.arg("password") == validPassword) {
      // redirect to main page
      server.sendHeader("Location", "/");
      server.send(303);
      LEDv.SetValue(HIGH,Digital);
      LEDl.SetValue(LOW,Digital);
      attempts=0;
      return;
    }
    else
    {
      attempts++;
      if (attempts >= 3)
      {
        LEDl.SetValue(HIGH,Digital); // notify the house if there was 3 attempts or more to login to panel
      }
    }
  }
  // redirect to login page
  server.sendHeader("Location", "/login");
  server.send(303);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/getvalue", handleGetValue);
  server.on("/login", handleLogin_Page);
  server.on("/loginvalid",validateLogin);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  
  server.handleClient();
}