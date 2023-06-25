#include"Tub.h"

extern uint8_t drum_status_buffer;
Tub::Tub(){}
Tub::Tub(uint8_t sensor_pin){
  this->sensor_pin = sensor_pin;
  this->state = TUB_OFF;

}
void Tub::Init(){
  this->state = TUB_INIT;
  pinMode(this->sensor_pin, INPUT);
  uint8_t sum = 0;
  this->index = 0;
  this->tub_state = EMPTY;
  this->raw = 0;
 
  for (int i = 0 ; i < 10; i++){

    this->raw = analogRead(this->sensor_pin);
    if (this->raw > 950){
      this->read_values[i] = 0;
    }else
      this->read_values[i] = 1;
    
    sum+= this->read_values[i];
  }
  this->tub_state = sum/10; 
  drum_status_buffer = this->tub_state;
  this->state = TUB_FULL;

}
uint8_t Tub::Debounce(){
  uint8_t sum = 0;
  for (int i = 0 ; i < 10; i++){
  
    sum+= this->read_values[i];

  }
  return sum/10;
}


void Tub::Read_tub(){
  
  this->raw = analogRead(this->sensor_pin);

  if (this->raw > 900){
    this->read_values[this->index] = 0;
  }else
    this->read_values[this->index] = 1;
  
  this->index++;
  this->index = this->index%10  ;

  this->tub_state = this->Debounce();
  drum_status_buffer = this->tub_state; 

}
