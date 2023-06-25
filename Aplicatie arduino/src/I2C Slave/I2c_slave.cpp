#include "I2c_slave.h"

// globals for location data, mostly for debugging
extern  uint8_t location_data_buffer[3][3];
extern  uint8_t filtered_measurements_buffer[3];
extern  uint8_t last_movement_decision_buffer;

// globals for battery management
extern  float battery_voltage_buffer;

// locals for drum data
extern  uint8_t drum_status_buffer;

I2c_slave::I2c_slave(){}
I2c_slave::I2c_slave(uint16_t device_address, void(*ISR_RX) (const uint8_t data[], const uint8_t length),void(*ISR_TX) ()){

  this->device_address = device_address;
  this->ISR_RX = ISR_RX;
  this->ISR_TX = ISR_TX;

}
void I2c_slave::Init(){
  this->battery_voltage.float_value = 0;
  this->request = 0;
  this->message[16] = {0};
  this->transfer_status = -1;
  this->g_twi.SetLocalDeviceAddress(device_address);
  this->g_twi.SetSlaveTransmitHandler(ISR_TX);
  this->g_twi.SetSlaveReceiveHandler(ISR_RX);
  this->state = I2C_SLAVE_IDLE;
  // the rest of the module is based solely on interrupts
}
I2c_slave_state I2c_slave::Get_state(){

  return this->state;
}
void I2c_slave::Set_state(const I2c_slave_state state){

  this->state = state;
}
uint8_t I2c_slave::Get_status(){
  return this->transfer_status;
}
void I2c_slave::Set_status(const uint8_t status){

  this->transfer_status = status;
}
uint8_t I2c_slave::Get_request(){
  
  return this->request;
}
void I2c_slave::Set_request(const uint8_t request){

  this->request = request;
}
void I2c_slave::Clear_message(){

  memset (this->message,0, 16);
  this->message_size = 0;
}
uint8_t* I2c_slave::Get_message(){

    return this->message;

}
uint8_t I2c_slave::Get_message_size(){

  return this->message_size;

}
void I2c_slave::Compile_message(){

  uint8_t i = 0;
  uint8_t it = 0;
  uint8_t jt = 0;
  switch (this->Get_state()){
    case I2C_OFF: {
      Serial.println("I2c error at compile messsage I2C_OFF");
      break;
    }
    case I2C_SLAVE_IDLE: {
      Serial.println("I2c error at compile messsage  DRIVER IS IDLE");
      break;
    }
    case I2C_MESSAGE_COMPILED: {
      Serial.println("I2c error at compile messsage  MESSAGE IS ALLREADY COMPILED");
      break;
    }
    case I2C_DISTANCE_DATA_REQUESTED: {
      
      for (it = 0 ; it < 3 ; it ++){
        
        for (jt = 0 ; jt < 3 ; jt ++){
        
          this->message[i] = location_data_buffer[it][jt];
          i ++;
        }
        
      }
      memset(location_data_buffer,0, sizeof(location_data_buffer));
      
      for (it = 0 ; it < 3 ; it ++){
        
        this->message[i] = filtered_measurements_buffer[it];
        i ++;
      }
      memset(filtered_measurements_buffer,0, sizeof(filtered_measurements_buffer));
      
      this->message[i] = last_movement_decision_buffer;
      i ++;
      this->message_size = i;
      memset(&last_movement_decision_buffer,0, sizeof(last_movement_decision_buffer));
      
      this->Set_state(I2C_MESSAGE_COMPILED);
    break;
    }
    case I2C_BATTERY_DATA_REQUESTED: {

      this->battery_voltage.float_value = battery_voltage_buffer;
      memset(&battery_voltage_buffer,0 , sizeof(battery_voltage_buffer));

      for (int it = 0 ; it < 4 ; it ++){
      this->message[it] = this->battery_voltage.bytes[it]; // cursed way to send floats over i2c keep in mind the bites get flipped
      i++;
      }

      this->message_size = i;
      this->Set_state(I2C_MESSAGE_COMPILED);    
    break;
    }
    case I2C_DRUM_DATA_REQUESTED: {
    
      this->message[i] = drum_status_buffer;
      i++;
      this->message_size = i;

      memset(&drum_status_buffer,0 , sizeof(drum_status_buffer));
      this->Set_state(I2C_MESSAGE_COMPILED);
    break;
    }
  }
}