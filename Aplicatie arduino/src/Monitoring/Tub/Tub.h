#ifndef TUB_H
#define TUB_H
#include <Arduino.h>

#define EMPTY 0
#define FULL 1

enum tub_state{

  TUB_OFF = 0,
  TUB_INIT = 1,
  TUB_FULL = 2,
  
};

class Tub{
  
  private:
  uint8_t sensor_pin;
  uint8_t index;
  uint16_t raw;
  uint8_t read_values[10];
  enum tub_state state;
  uint8_t Debounce();
  
  public:
  uint8_t tub_state;

  Tub();
  Tub(uint8_t sensor_pin);
  void Init();
  void Read_tub();
};


#endif