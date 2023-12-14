#include "Helper.h"
#include "Auth.h"

#include "Smart.h"
#include "Pages.h"

/*
  Todo:
    [X] Set and get cookies (auth)
    [X] add buzzer for alarm
    [ ] add ultrasonic module (for parking of course)

*/
// WC to connect to wifi
// AP to work as AP
#define WC

// Photocell
Module PC(A0,true);
// Valid login led
Module LEDv(D0,false);
// Not valid login led
Module LEDl(D1,false);

// SSID and password for nodemcu
const char* ssid = "ali";
const char* password = "12345689";



// Something for debuging
const int ledPin = LED_BUILTIN;
int ledState = LOW;
int pval = 0;

// Valid username and password for login
const char* validUsername = "ali";
const char* validPassword = "1234";

// Login attempts (3 Max)
short attempts = 0;

// The main page
void handleRoot() {
  is_authenticated("/login");
  String html = "<html><body>";
  html += "<h1 id='photocell'>Photocell: 0</h1>";
  html += "<h1 id='distance'>Distance: 0</h1>";
  html += "<button onclick=\"togglePin()\">Toggle LED</button>";
  html += " <a href=\"/login?DIS=1\">Disconnect</a>";
  html += "<script>function togglePin(){var xhr=new XMLHttpRequest();xhr.open('GET','/setvalue',true);xhr.send();}</script>";
  html += "<script>function updateValue() {var xhr = new XMLHttpRequest();xhr.onreadystatechange = function () {if (xhr.readyState === 4 && xhr.status === 200) {var data = xhr.responseText;console.debug(data);var values = data.split(';');for (var i = 0; i < values.length; i++) {var keyValue = values[i].split(':');var key = keyValue[0];var value = keyValue[1];if (key && value) {document.getElementById(key).innerHTML = key + ': ' + value;}}}};xhr.open('GET', '/getvalue', true);xhr.send();}</script>";
  html += "<script>setInterval(updateValue, 1000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void haldleSetValue()
{
  is_authenticated("/login");
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  server.send(200, "text/plain", "LED toggled");
}
void handleLogin_Page()
{
  if (server.hasArg("DIS")) {
    deauth("/login");
    return;
  }
  server.send(200, "text/html", p_login);
}

// Get module value everytime function called
void handleGetValue() {
  String data = "";
  data += "photocell:";
  data += PC.GetValue<int>(Analog);
  data += ";";
  data += "distance:";
  data += 0;
  data += ";";
  
  server.send(200, "text/plain", String(data));
}
// validate posted args for login user
void validateLogin()
{
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == validUsername && server.arg("password") == validPassword) {
      // redirect to main page
      auth("/");
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

  #ifdef AP
    Serial.println("Creating AP...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
  #else
    Serial.println("Connecting to AP...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    Serial.print("Client IP address: ");
    Serial.println(WiFi.localIP());
  #endif

  server.on("/", handleRoot);

  server.on("/login", handleLogin_Page);
  server.on("/loginvalid",validateLogin);

  server.on("/setvalue", haldleSetValue);
  server.on("/getvalue", handleGetValue);

  
  server.onNotFound([](){server.send(404, "text/plain", "404 Room not found :/");});

  server.collectHeaders("Cookie");

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}