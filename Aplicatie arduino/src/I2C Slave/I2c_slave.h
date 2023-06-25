#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H
#include <Arduino.h>
#include <nI2C.h>
#include "I2c_slave_ISR.h"

enum I2c_slave_message_codes{

  DISTANCE_DATA_REQUEST = 0X02,
  BATTERY_DATA_REQUEST = 0X04,
  DRUM_DATA_REQUEST = 0X08,
  FAN_OFF_REQUEST = 0X20, 
  FAN_ON_REQUEST = 0X40, 
  SYSTEM_SHUTDOWN_REQUEST = 0X80 

};
enum I2c_slave_state {

  I2C_OFF  = 0,
  I2C_SLAVE_IDLE = 1,
  I2C_DISTANCE_DATA_REQUESTED = 2,   
  I2C_BATTERY_DATA_REQUESTED = 4, 
  I2C_DRUM_DATA_REQUESTED = 8,  
  I2C_MESSAGE_COMPILED = 6
};
union float_to_bytes{

  byte bytes[4];
  float float_value;
};
class I2c_slave {
  
  private:

  uint16_t device_address;
  void (*ISR_RX) (const uint8_t data[], const uint8_t length);
  void (*ISR_TX) ();
  union float_to_bytes battery_voltage;
  uint8_t request;
  uint8_t transfer_status;
  uint8_t message_size;
  uint8_t message[16];
  I2c_slave_state state;

  public:

  CTWI g_twi;
  // constructors
  I2c_slave();
  I2c_slave(uint16_t device_address, void(*ISR_RX) (const uint8_t data[], const uint8_t length),void(*ISR_TX) ());
  // init functions
  void Init();
  // getters/setters
  uint8_t Get_request();
  void Set_request(const uint8_t request);
  uint8_t Get_status();
  void Set_status(const uint8_t status);
  I2c_slave_state Get_state();
  void Set_state(const I2c_slave_state state);
  uint8_t* Get_message();
  uint8_t Get_message_size();
  void Clear_message();
  // function that deals with data output
  void Compile_message();
};
#endif