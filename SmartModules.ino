#include "Smart.h"

Module LED(LED_BUILTIN,false);
Module LED2(12,false);
Module Photocell(A0,true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  int light = Photocell.GetValue<int>(Analog);
  if (light < 80)
    LED2.SetValue(HIGH, Digital);
  else
    LED2.SetValue(LOW, Digital);
  LED.SetValue(HIGH, Digital);
  //LED2.SetValue(HIGH, Digital);
  delay(500);
  LED.SetValue(LOW, Digital);
  //LED2.SetValue(LOW, Digital);
  delay(500);
}
