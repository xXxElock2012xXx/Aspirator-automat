#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include <Arduino.h>
#include "Ultrasonic_ISR.h"
#define ULTRASONIC_DEBUGGING false
 enum Ult_state {
  ULTRASONIC_OFF = 0,
  ULTRASONIC_IDLE = 1,
  ULTRASONIC_ACQUISITION_TRIGGERED = 2,
  ULTRASONIC_ACQUISITION_ECHO_HIGH = 3,
  ULTRASONIC_ACQUISITION_DONE = 4
};

enum Error{
  PING_OUT_OF_RANGE = 0,
  SUCCESFULL_TRIGGER = 1,
  SENSOR_NOT_TRIGGERED = 2,
  RESULTS_NOT_SAMPLED = 3,
  DRIVER_OFF = 4,
  ULTRASONIC_ERROR = 5
};

class Ultrasonic{
uint8_t trig_pin;
uint8_t echo_pin;
void (*ISR) ();
static constexpr float sound_speed = 0.017f;
enum Ult_state state;
unsigned long long int t1;
unsigned long long int t2;
uint8_t measured_distance;
public:
char name[30];
public:
Ultrasonic ();
Ultrasonic (const char *name, uint8_t trig_pin, uint8_t echo_pin, void(*ISR)());
void Init();
enum Error Trigger();
enum Ult_state Get_state ();
void Set_state (enum Ult_state state);
float Get_sound_speed ();
void Set_measured_distance(uint8_t distance);
uint8_t Get_measured_distance();
unsigned long long int Get_t1 ();
void Set_t1 (unsigned long long int value);
unsigned long long int Get_t2 ();
void Set_t2 (unsigned long long int value);
};

#endif