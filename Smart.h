/*
  Smart home library
  Ali Nakhaee
*/

#ifndef SMART_H
#define SMART_H
#include <Arduino.h>

enum PinType {
    Analog,
    Digital
};

class Module
{
private:
  unsigned short m_pin;
  bool m_isInput;
public:
  Module(unsigned short pin, bool isInput);

  template <typename T>
  void SetValue(T value, PinType type)
  {
    if (m_isInput)
      return;
    if (type == Analog)
      analogWrite(m_pin,value);
    else
      digitalWrite(m_pin,value);
  }
  template <typename T>
  T GetValue(PinType type)
  {
    if (!m_isInput)
      return 0;
    T val;
    if (type == Analog)
      val = analogRead(m_pin);
    else
      val = digitalRead(m_pin);
    return val;
  }
};

Module::Module(unsigned short pin, bool isInput)
{
  m_isInput = isInput;
  if (m_isInput)
    pinMode(pin, INPUT);
  else
    pinMode(pin, OUTPUT);

  m_pin = pin;
}

#endif