#include "Arduino.h"
#include "Monitoring.h"

extern uint8_t recieved_messages_buffer;

  Monit::Monit(){}

  Monit::Monit(uint8_t shutoff_pin, Relay *cooling_fan_relay, Battery *Battery_pack, Tub *Vacuum_tub){
    this->state = MONITORING_OFF;
    this->shutoff_pin = shutoff_pin; 
    this->cooling_fan_relay =  cooling_fan_relay;
    this->Battery_pack = Battery_pack;
    this->Vacuum_tub = Vacuum_tub;
  }
  void Monit::Init(){

    this->state = MONITORING_INIT;

    pinMode(this->shutoff_pin, OUTPUT);
    digitalWrite(this->shutoff_pin, HIGH); // at this point the board is powered on independently
    
    this->cooling_fan_relay->Init();
    this->Battery_pack->Init();
    this->Vacuum_tub->Init();
    
    this->state = MONITORING_FULL;
  }

  void Monit::Assert_state(){

    this->Battery_pack->Read_voltage();
    this->Vacuum_tub->Read_tub();

  }



  void Monit::Process_requests(){

    if ((recieved_messages_buffer & COOLING_FAN_ON_MASK) == COOLING_FAN_ON_MASK ){
      this->cooling_fan_relay->Switch(TURN_ON); // service the request
      recieved_messages_buffer = (recieved_messages_buffer & (~COOLING_FAN_ON_MASK)); // clear the request 
    }

    if ((recieved_messages_buffer & COOLING_FAN_OFF_MASK) == COOLING_FAN_OFF_MASK){
      this->cooling_fan_relay->Switch(TURN_OFF); // service the request
      recieved_messages_buffer = (recieved_messages_buffer & (~COOLING_FAN_OFF_MASK)); // clear the request
    }

    if ((recieved_messages_buffer & SHUTOFF_REQUEST) == SHUTOFF_REQUEST){
      digitalWrite(this->shutoff_pin, LOW); // kill the power supply
    
    }

  }