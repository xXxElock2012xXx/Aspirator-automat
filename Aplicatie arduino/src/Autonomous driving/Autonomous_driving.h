#ifndef AUTONOMOUS_DRIVING_H
#define AUTONOMOUS_DRIVING_H
#include <Arduino.h>
#include "Powertrain/Powertrain.h"
#include "Ultrasonic/Ultrasonic.h"

#define Autodrv_debug false
#define Minimum_allowed_distance 30
#define Left_distance (this->filtered_measurements[0])
#define Front_distance (this->filtered_measurements[1])
#define Right_distance (this->filtered_measurements[2])

enum Autodrv_state {
  AUTODRV_OFF = 0,
  AUTODRV_INIT = 1,
  AUTODRV_MEASURING_LEFT = 2,
  AUTODRV_MEASURING_FRONT = 3,
  AUTODRV_MEASURING_RIGHT = 4,
  AUTODRV_FILTERING_AND_STEERING = 5
  
};
enum Autodrv_loc_state{
  AUTODRV_FIRST_FORWARD = 0,
  AUTODRV_FORWARD = 1,
  AUTODRV_FIRST_LEFT = 2,
  AUTODRV_LEFT = 3,
  AUTODRV_FIRST_RIGHT = 4,
  AUTODRV_RIGHT = 5
};

extern unsigned long long int startup_CurrMs;
extern unsigned long long int startup_PrevMs;
class Autodrv {

  Ultrasonic* ult_left;
  Ultrasonic* ult_front;
  Ultrasonic* ult_right;
  Powertrain* Pwrtrain;
  uint8_t sampling_matrix[3][3];
  uint8_t location_data[3][3];
  uint8_t filtered_measurements[3];
  uint8_t current_step;
  enum Autodrv_state state;
  enum Autodrv_loc_state last_movement_decision;
  uint8_t sampling_column;

  public:
  Autodrv();
  Autodrv(Ultrasonic* sensor1, Ultrasonic* sensor2, Ultrasonic* sensor3, Powertrain* powertrain);
  void Autodrv_MainTask();
  void Autodrv_Init();
  void Autodrv_Export_Location_Data();
  void Autodrv_Export_last_movement_decision();
  void Autodrv_Export_Filtered_Measurements();
  void Autodrv_Move(PwrTraind_Direction direction);

};


#endif