#include"Battery.h"

extern float battery_voltage_buffer;

Battery::Battery(){}
Battery::Battery(uint8_t voltage_pin){

  this->voltage_pin = voltage_pin;

}

void Battery::Init(){
  
  float sum = 0;
  pinMode(this->voltage_pin, INPUT);
  this->voltage = 0.0;
  this->raw_voltage = 0;

  for (this->index = 0; this->index < 10; this->index++){

    this->raw_voltage = analogRead(this->voltage_pin); 
    this->read_values[this->index] = (raw_voltage/1024*5)*5.50;
    sum += this->read_values[this->index];
  }

  this->voltage = sum/10; 
  battery_voltage_buffer = this->voltage;
  this->index = 0;
}

void Battery::Read_voltage(){ // 5.63

  float sum = 0.0;
  float voltage;
  this->raw_voltage = analogRead(this->voltage_pin); 
  voltage = (this->raw_voltage/1024)*5*5.50; 
  if(voltage > 9.6){

    if (voltage  <  12.6)
      this->read_values[this->index] = (this->raw_voltage/1024*5)*5.50;
    else 
      this->read_values[this->index] = 12.6;
    this->index++;

  }
  for (int i = 0 ; i < this->index; i++){
    sum += this->read_values[i];

  }
  if (this->index){
    this->voltage = sum/(this->index);
    battery_voltage_buffer = this->voltage;
    
  }
  
  this->index = this->index % Number_of_samples ;

}