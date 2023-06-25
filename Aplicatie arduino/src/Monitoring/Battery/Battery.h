#ifndef BATTERY_H
#define BATTERY_H
#include<Arduino.h>

#define Number_of_samples 100

class Battery {
private:
uint8_t voltage_pin;
float raw_voltage;
uint8_t index;
float read_values[Number_of_samples];
public:
float voltage;

  Battery();
  Battery(uint8_t voltage_pin);

  void Init();
  void Read_voltage();


};

#endif