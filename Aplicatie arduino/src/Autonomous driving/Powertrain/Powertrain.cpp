#include "Powertrain.h"
Powertrain::Powertrain (){}
Powertrain::Powertrain (Hbridge* motor_stanga, Hbridge* motor_dreapta){
  this->motor_stanga = motor_stanga;
  this->motor_dreapta = motor_dreapta;
  
}
void Powertrain::Move_forward(uint8_t speed){
  motor_stanga->Set_speed(speed);
  motor_dreapta->Set_speed(speed);
  motor_stanga->Set_direction(H_bridge_forward);
  motor_dreapta->Set_direction(H_bridge_forward);
  noInterrupts();
  this->direction = Powertrain_forward;
  motor_stanga->Update_locomotion();
  motor_dreapta->Update_locomotion();
  interrupts();
}

void Powertrain::Init(){
  motor_stanga->Init();
  motor_dreapta->Init();
  this->direction = Powertrain_no_direction;  
}
void Powertrain::Fixed_turn(PwrTraind_Direction direction,uint8_t speed){
  this->direction = direction;
  motor_stanga->Set_speed(speed);
  motor_dreapta->Set_speed(speed); // set the speed  
  switch(this->direction){ // decide where you are going
    case Powertrain_left:{// stanga
    motor_stanga->Set_direction(H_bridge_backward);
    motor_dreapta->Set_direction(H_bridge_forward);
    }break;
    case Powertrain_right:{// dreapta
    motor_stanga->Set_direction(H_bridge_forward);
    motor_dreapta->Set_direction(H_bridge_backward);
    }break;
    case Powertrain_forward:{
#if Powertrain_debug == true
      Serial.println(" Powertrain fixed turn called with wrong parameters");
#endif
    }break;
    case Powertrain_no_direction:{
#if Powertrain_debug == true
      Serial.println(" Powertrain fixed turn called with wrong parameters");
#endif
    }break;
  }
  motor_stanga->Update_locomotion(); // go 
  motor_dreapta->Update_locomotion();
}
/*
void Powertrain::Curbed_turn(PwrTraind_Direction direction,uint8_t speed_left,uint8_t speed_right){ // untested

  motor_dreapta->Set_direction(H_bridge_forward); // both motors need to face forward
  motor_stanga->Set_direction(H_bridge_forward);

  motor_stanga->Set_speed(speed_left);
  motor_dreapta->Set_speed(speed_right); // set the speeds this will actually determine the direction  


  motor_stanga->Update_locomotion(); // go 
  motor_dreapta->Update_locomotion();

}
*/