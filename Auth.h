#ifndef AUTH_H
#define AUTH_H

#include "Helper.h"

// Check if header is present and correct
bool is_authenticated(const char* path) {
  Serial.println("Check Authentication");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("auth=1") != -1) {
      Serial.println("Authentication Successful");
      return true;
    }
  }
  Serial.println("Authentication Failed");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Location", path);
  server.send(303);
  return false;
}

void deauth(const char* path)
{
  Serial.println("Disconnection");
  server.sendHeader("Location", path);
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Set-Cookie", "auth=0");
  server.send(303);
  return;
}

void auth(const char* path)
{
  server.sendHeader("Location", path);
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Set-Cookie", "auth=1");
  server.send(303);
  Serial.println("Log in Successful");
  return;
}



#endif