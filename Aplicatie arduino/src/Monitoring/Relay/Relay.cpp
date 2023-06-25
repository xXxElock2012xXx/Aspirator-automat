  #include "Relay.h"
  Relay::Relay(){}
  Relay::Relay(uint8_t in_pin, const char *controlled_device){

    strcpy(this->Controlled_device, controlled_device);
    this->in_pin = in_pin;
    this->state = RELAY_OFF;

  }

  void Relay::Init(){
    pinMode(this->in_pin, OUTPUT);
    digitalWrite(this->in_pin, TURN_OFF);
    this->state = TURN_OFF;
  }
  void Relay::Switch(uint8_t on_off){
    digitalWrite(this->in_pin, on_off);
    this->state = (Relay_state)on_off;
  
  }



  