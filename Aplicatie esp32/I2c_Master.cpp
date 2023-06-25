#include "esp32-hal.h"
#include "I2c_Master.h"
#include <iomanip>
extern uint8_t location_data[3][3];
extern uint8_t filtered_measurements[3];
extern uint8_t last_movement_decision;
extern uint8_t drum_state;
extern float battery_voltage_buf;
extern int battery_percentage_buf;
extern float temperature;
extern char sensors[3][20];
extern char directions[5][20];
uint8_t fan_state;

int mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


I2cMaster::I2cMaster(){}
I2cMaster::I2cMaster(Adafruit_SSD1306 *display, Adafruit_HTU21DF *temperature_sensor, uint8_t slave_device_address){
  this->display = display;
  this->temperature_sensor = temperature_sensor;
  this->slave_device_address = slave_device_address;

}
void I2cMaster::Init(){

  this->battery_voltage.float_value = 0.0;
  this->command = NO_COMMAND;
  
  Wire.begin();
  // initialize components
  if(!this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  for(;;);
  }

  if (!this->temperature_sensor->begin()) {
    Serial.println("Couldn't find sensor!");
  for (;;);
  }


  //after init display a nice message
  this->display->clearDisplay();
  this->display->setTextSize(1);
  this->display->setTextColor(WHITE);
  this->display->setCursor(0,0);
  this->display->print("Init finished");
  this->display->display(); 
  delay(100);
}

void I2cMaster::Send_command(enum I2c_commands command){

  this->command = (enum I2c_commands)(this->command | (command & 0x0F)); // add command to the list of recieved data

  switch (command){
    case NO_COMMAND:{
      Serial.print("wrong parameter call");
      break;
    }
    
    case TEMPERATURE_DATA_REQUEST:{
      
      temperature = this->temperature_sensor->readTemperature();
      
      break;
    }


    case DISTANCE_DATA_REQUEST:{
      Wire.beginTransmission(this->slave_device_address); 
      Wire.write((uint8_t)DISTANCE_DATA_REQUEST);  
      Wire.endTransmission();
      delayMicroseconds(100);
      Wire.requestFrom((int)this->slave_device_address, 13);
      
      while (Wire.available()) { // we expect 13 bytes to be sent 
        
        for (int i = 0 ; i < 3 ; i++){
          for (int j = 0; j < 3 ; j++){
            location_data[i][j] = Wire.read();

          }
          
        }

        for (int i = 0; i < 3; i++){
          filtered_measurements[i] = Wire.read();

        }

        last_movement_decision =  Wire.read();

      }

      break;
    }
    case BATTERY_DATA_REQUEST:{
      int i = 0;
      Wire.beginTransmission(this->slave_device_address);
      Wire.write((uint8_t)BATTERY_DATA_REQUEST);  
      Wire.endTransmission();
      delayMicroseconds(100);
      Wire.requestFrom((int)this->slave_device_address, 4);
      
      while (Wire.available()) { // we expect 4 bytes
        this->battery_voltage.bytes[i] = Wire.read();
        i++;
      }
      battery_voltage_buf = this->battery_voltage.float_value;

      break;
    }
    case DRUM_DATA_REQUEST:{
      Wire.beginTransmission(this->slave_device_address); 
      Wire.write((uint8_t)DRUM_DATA_REQUEST);  
      Wire.endTransmission();
      delayMicroseconds(100);
      Wire.requestFrom((int)this->slave_device_address, 1);
      
      while (Wire.available()) { // we expect 1 bytes
        drum_state = Wire.read();
        
      }
      

      break;
    }
    case FAN_OFF_REQUEST:{
      Wire.beginTransmission(this->slave_device_address); 
      Wire.write((uint8_t)FAN_OFF_REQUEST);  
      Wire.endTransmission();

      break;
    }
    case FAN_ON_REQUEST:{
      Wire.beginTransmission(this->slave_device_address); 
      Wire.write((uint8_t)FAN_ON_REQUEST);  
      Wire.endTransmission();
      break;
    }
    case SYSTEM_SHUTDOWN_REQUEST:{ // after this the entire project gets powered down
      Wire.beginTransmission(this->slave_device_address); 
      Wire.write((uint8_t)SYSTEM_SHUTDOWN_REQUEST);  
      Wire.endTransmission();

      break;
    }
    

  }
}

void I2cMaster::Process_data(){

  //clear display
  this->display->clearDisplay();
  this->display->setCursor(0, 0);
 

  if ((this->command & DISTANCE_DATA_REQUEST) == DISTANCE_DATA_REQUEST){
    for (int i = 0 ; i < 3 ; i++){
      for (int j = 0; j < 3 ; j++){
        
        //this->display->print(String(location_data[i][j]) + " "); 
        
      }
      this->display->print(sensors[i]);
      this->display->print("|");
      this->display->println(String(filtered_measurements[i]) + " ");
        
    }
    this->display->println("------------");
    this->display->println(directions[last_movement_decision]);

    // clear the request as it has been serviced
    this->command = (I2c_commands)(this->command & ~DISTANCE_DATA_REQUEST);
  
  }

  if ((this->command & BATTERY_DATA_REQUEST) == BATTERY_DATA_REQUEST){
    
    this->display->print(battery_voltage_buf);
    this->display->print("  ");
    
    if (battery_voltage_buf > 9.6){
      battery_percentage_buf = mapf(battery_voltage_buf,9.6,12.6,0.0,100.0);
      if (battery_voltage_buf < 9.6 ){
        Serial.println("Baterie");
        Send_command(SYSTEM_SHUTDOWN_REQUEST);
      }
    }

    this->display->print(battery_percentage_buf);
    this->display->println("%");
    // clear the request as it has been serviced
    this->command = (I2c_commands)(this->command & ~BATTERY_DATA_REQUEST);
  }
  
  if ((this->command & DRUM_DATA_REQUEST) == DRUM_DATA_REQUEST){

    this->display->println(drum_state);
    // clear the request as it has been serviced
    this->command = (I2c_commands)(this->command & ~DRUM_DATA_REQUEST);
  
    if (drum_state == 1){
      Serial.println("CUVA ASPIRATOR");
      Send_command(SYSTEM_SHUTDOWN_REQUEST);
    }
  }
  if ((this->command & TEMPERATURE_DATA_REQUEST) == TEMPERATURE_DATA_REQUEST){

    this->display->println(temperature);

    if (temperature > 30 && fan_state == 0){

      this->Send_command(FAN_ON_REQUEST);
      fan_state = 1;

    }else if (temperature < 25 && fan_state == 1 ){

      this->Send_command(FAN_OFF_REQUEST);
      fan_state = 0;

    }
    // clear the request as it has been serviced
    this->command = (I2c_commands)(this->command & ~TEMPERATURE_DATA_REQUEST);
  }

  this->display->display(); 
}