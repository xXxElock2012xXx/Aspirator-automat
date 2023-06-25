#include "H_bridge.h"
Hbridge::Hbridge(){

}
Hbridge::Hbridge (uint8_t ena,uint8_t in1,uint8_t in2){
  this->ena = ena;
  this->in1 = in1;
  this->in2 = in2;
}
void Hbridge::Init(){
  pinMode(ena,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  this->speed = 0;
  this->direction = H_bridge_no_direction;
}

void Hbridge::Set_direction (uint8_t direction){
  this->direction = direction;
}
int8_t Hbridge::Get_direction (){
  return this->direction;
}
void Hbridge::Set_speed (uint8_t speed){
  this->speed = speed;
}
uint8_t Hbridge::Get_speed (){
  return this->speed;
}
void Hbridge::Update_locomotion(){

  switch(this->direction){

    case H_bridge_forward:{//forward
    //write to pins
      analogWrite(this->ena,speed);
      digitalWrite(this->in1,LOW);
      digitalWrite(this->in2,HIGH);
    break;
    }
    case H_bridge_backward:{//backwards
      //write to pins
      analogWrite(this->ena,speed);
      digitalWrite(this->in1,HIGH);
      digitalWrite(this->in2,LOW);
    break;
    }
  }
}

