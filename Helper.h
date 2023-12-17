#ifndef HELPER_H
#define HELPER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
// //Static IP address configuration
// IPAddress staticIP(192, 168, 18, 12); //ESP static ip
// IPAddress subnet(255, 255, 255, 0);  //Subnet mask

// Running server on port 80
ESP8266WebServer server(80);

#endif