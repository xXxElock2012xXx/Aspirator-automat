#ifndef MONITORING_H
#define MONITORING_H
#include "Relay/Relay.h"
#include "Battery/Battery.h"
#include "Tub/Tub.h"
#include <Arduino.h>

#define COOLING_FAN_OFF_MASK 0x20
#define COOLING_FAN_ON_MASK 0x40 
#define SHUTOFF_REQUEST 0x80 


enum monit_state{

  MONITORING_OFF = 0,
  MONITORING_INIT = 1,
  MONITORING_FULL = 2,
  
};

class Monit{
  
  private:
  uint8_t shutoff_pin;
  Battery *Battery_pack;
  Tub *Vacuum_tub;
  Relay *cooling_fan_relay;
  enum monit_state state;
  public:
  Monit();
  Monit(uint8_t shutoff_pin, Relay *cooling_fan_relay, Battery *Battery_pack, Tub *Vacuum_tub);
  void Init();
  void Assert_state();
  void Process_requests();
};

#endif