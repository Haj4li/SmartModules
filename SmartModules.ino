#include "Helper.h"
#include "Auth.h"

#include "Smart.h"
#include "Pages.h"

/*
  Todo:
    [X] Set and get cookies (auth)
    [X] add buzzer for alarm
    [x] add ultrasonic module (for parking of course)

*/


// WC to connect to wifi
// AP to work as AP
#define AP
#define interval 1000


unsigned long timer = 0;

// Photocell
Module PC(A0,true);
// Valid login led
Module LEDv(D0,false);
// Not valid login led
Module ALARM(D1,false);
// Flame
Module FLAME(D2,true);
// parking led
Module PARKINGLED(D5,false);

// ultrasonic
Module UTRIG(D3,false);
#define echoPin D4

unsigned int distance = 0;
unsigned int gas = 0;



// SSID and password for nodemcu
const char* ssid = "Homino";
const char* password = "h1234567";



// Something for debuging
int ledState = LOW;

// Valid username and password for login
const char* validUsername = "ali";
const char* validPassword = "1234";

// Login attempts (3 Max)
short attempts = 0;


void GetData()
{
  unsigned long currentmilies = millis();
  if (FLAME.GetValue<int>(Digital) == LOW || gas > 700)
  {
    ALARM.SetValue(HIGH,Digital);
  }
  else
  {
    if (attempts < 3)
      ALARM.SetValue(LOW,Digital);
  }
  if (currentmilies - timer > interval)
  {
    timer = currentmilies;
    // distance 
    
    UTRIG.SetValue(HIGH, Digital);
    delayMicroseconds(10);
    UTRIG.SetValue(LOW, Digital);
    distance =  pulseIn(echoPin, HIGH) * 0.017;
    if (distance < 60 && distance > 17)
    {
      PARKINGLED.SetValue(HIGH,Digital);
    }
    else
    {
      PARKINGLED.SetValue(LOW,Digital);
    }
    


  }

}

// The main page
void handleRoot() {
  is_authenticated("/login");
  String html = "<html><body>";

  html += "<h1 id='Gas'>Gas: 0</h1>";
  html += "<h1 id='Distance'>Distance: 0</h1>";
  html += "<h1 id='House'>House: 0</h1>";

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
  LEDv.SetValue(ledState,Digital);
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
  data += "Gas:";
  gas = PC.GetValue<int>(Analog);
  data += gas;
  data += ";";
  data += "Distance:";
  data += distance;
  data += ";";
  data += "House:";
  if (FLAME.GetValue<int>(Digital) == LOW)
  {
    data += "ON FIRE!";
  }
  else if (gas > 840)
  {
    data += "GAAAAS You are about to die (slowly)";
  }
  else
  {
    data += "Normal";
  }
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
      if (FLAME.GetValue<int>(Digital) != HIGH || gas <= 700)
        ALARM.SetValue(LOW,Digital);
      attempts=0;
      return;
    }
    else
    {
      attempts++;
      if (attempts >= 3)
      {
        //Serial.println("Error..");
        ALARM.SetValue(HIGH,Digital); // notify the house if there was 3 attempts or more to login to panel
      }
    }
  }
  // redirect to login page
  server.sendHeader("Location", "/login");
  server.send(303);
}

void setup() {
  //Serial.begin(115200);
  pinMode(echoPin, INPUT); 

  #ifdef AP
    //Serial.println("Creating AP...");
    WiFi.mode(WIFI_AP);
    //WiFi.softAPConfig(staticIP, NULL, subnet);
    WiFi.softAP(ssid, password);

    IPAddress apIP = WiFi.softAPIP();
    //Serial.print("AP IP address: ");
    //Serial.println(apIP);
  #else
    //Serial.println("Connecting to AP...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    //Serial.print("Client IP address: ");
    //Serial.println(WiFi.localIP());
  #endif

  server.on("/", handleRoot);

  server.on("/login", handleLogin_Page);
  server.on("/loginvalid",validateLogin);

  server.on("/setvalue", haldleSetValue);
  server.on("/getvalue", handleGetValue);

  
  server.onNotFound([](){server.send(404, "text/plain", "404 Room not found :/");});

  server.collectHeaders("Cookie");

  server.begin();
  //Serial.println("HTTP server started");
}

void loop() {
  GetData();
  server.handleClient();
}