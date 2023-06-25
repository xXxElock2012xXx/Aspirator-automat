#ifndef RELAY_H
#define RELAY_H
#include <Arduino.h>
enum Relay_state {
  RELAY_OFF = -1,
  TURN_OFF = 0,
  TURN_ON = 1

};

class Relay{
  
  public:
  char Controlled_device[30];
  
  private:
  enum Relay_state state;
  uint8_t in_pin;
  
  public:
  Relay();
  Relay(uint8_t in_pin, const char *controlled_device);

  void Init();
  void Switch(uint8_t on_off);

};

#endif