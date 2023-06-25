#ifndef HBRIDGE_H
#define HBRIDGE_H
#include <Arduino.h>

#define H_bridge_forward 1u
#define H_bridge_backward 0u
#define H_bridge_no_direction -1

class Hbridge {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
  uint8_t speed;
  uint8_t direction;
  public:
  Hbridge();
  Hbridge (uint8_t ena,  uint8_t in1,uint8_t in2);
  void Set_direction (uint8_t direction);
  int8_t Get_direction ();
  void Set_speed (uint8_t speed);
  uint8_t Get_speed ();
  void Update_locomotion();
  void Init();
};
#endif