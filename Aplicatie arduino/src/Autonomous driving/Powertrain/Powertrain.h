#ifndef POWERTRAIN_H
#define POWERTRAIN_H
#include <Arduino.h>
#include "H bridge/H_bridge.h"
#define Powertrain_debug false
enum PwrTraind_Direction{
 Powertrain_left = -1, 
 Powertrain_forward = 0, 
 Powertrain_right = 1,
 Powertrain_no_direction = -2,  

};
class Powertrain {
  Hbridge* motor_stanga;
  Hbridge* motor_dreapta;
  PwrTraind_Direction direction;
  public:
  Powertrain();
  Powertrain(Hbridge* motor_stanga, Hbridge* motor_dreapta);
  void Move_forward(uint8_t speed);
  void Init();
  void Fixed_turn(PwrTraind_Direction direction,uint8_t speed);
  void Curbed_turn(PwrTraind_Direction direction,uint8_t speed_left,uint8_t speed_right);
};
#endif