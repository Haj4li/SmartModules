#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "Smart.h"
#include "Pages.h"
Module PC(A0,true);

const char* ssid = "KhaneHoshmand";
const char* password = "12345678";

ESP8266WebServer server(80);

int ledPin = LED_BUILTIN;
int ledState = LOW;
int pval = 10;

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

void handleGetValue() {
  pval = PC.GetValue<int>(Analog);
  Serial.println(pval);
  server.send(200, "text/plain", String(pval));
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

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  
  server.handleClient();
}