#ifndef I2C_MASTER_H
#define I2C_MASTER_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_HTU21DF.h"

union bytes_to_float{

  float float_value;
  uint8_t bytes[sizeof(float_value)];
};

enum I2c_commands{
  NO_COMMAND = 0X00,
  TEMPERATURE_DATA_REQUEST = 0X01,
  DISTANCE_DATA_REQUEST = 0X02,
  BATTERY_DATA_REQUEST = 0X04,
  DRUM_DATA_REQUEST = 0X08,
  FAN_OFF_REQUEST = 0X20,
  FAN_ON_REQUEST = 0X40,
  SYSTEM_SHUTDOWN_REQUEST = 0X80

};

enum I2c_Master_state{

  I2C_MASTER_OFF = 0,
  I2C_MASTER_IDLE = 1,
  I2C_MASTER_REQUESTS_SENT = 2,
  I2C_MASTER_ANSWER_RECIEVED = 3

};



class I2cMaster{
  private:
  Adafruit_SSD1306 *display;
  Adafruit_HTU21DF *temperature_sensor;
  bytes_to_float battery_voltage;
  uint8_t slave_device_address;
  enum I2c_commands command;

  public:
  I2cMaster();
  I2cMaster(Adafruit_SSD1306 *display, Adafruit_HTU21DF *temperature_sensor, uint8_t slave_device_address);
  void Init();
  void Process_data();
  void Send_command(enum I2c_commands command); // send commands individually

};

#endif