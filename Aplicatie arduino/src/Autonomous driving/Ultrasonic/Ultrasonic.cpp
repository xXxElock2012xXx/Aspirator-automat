#include "Ultrasonic.h"
#include "string.h"
Ultrasonic::Ultrasonic(){}
void Ultrasonic::Set_state (enum Ult_state state){
  this->state = state;
}

void Ultrasonic::Set_measured_distance(uint8_t distance){

  this->measured_distance = distance;
}
uint8_t Ultrasonic::Get_measured_distance(){
  uint8_t temp = 255;
  if (this->state == ULTRASONIC_ACQUISITION_DONE){
    noInterrupts();
    temp =  this->measured_distance;
    this->state = ULTRASONIC_IDLE;
    interrupts();
    this->measured_distance = 0;
    this->t1 = 0;
    this->t2 = 0;
  }
  return temp;
}

float Ultrasonic::Get_sound_speed (){
  return this->sound_speed;
}

enum Ult_state Ultrasonic::Get_state (){
return this->state;
}
unsigned long long int Ultrasonic:: Get_t1 (){
return this->t1;
}
void Ultrasonic::Set_t1 (unsigned long long int value){
  this->t1 = value;
}
unsigned long long int Ultrasonic:: Get_t2 (){
return this->t2;
}
void Ultrasonic::Set_t2 (unsigned long long int value){
  this->t2 = value;
}
Ultrasonic::Ultrasonic (const char *name,uint8_t trig_pin, uint8_t echo_pin, void(*ISR)()){
  strcpy(this->name, name);
  this->trig_pin = trig_pin;
  this->echo_pin = echo_pin;
  this->ISR = ISR;
  this->state = ULTRASONIC_OFF;
}
void Ultrasonic::Init(){
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  this->measured_distance = 0u;
  attachInterrupt(digitalPinToInterrupt(this->echo_pin), this->ISR, CHANGE);
  this->state = ULTRASONIC_IDLE;
}

enum Error Ultrasonic::Trigger(){
  enum Error retval = ULTRASONIC_ERROR;
  switch(this->state){
    case ULTRASONIC_IDLE:{
        noInterrupts();
        digitalWrite(this->trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->trig_pin, LOW);
        this->state = ULTRASONIC_ACQUISITION_TRIGGERED;
        retval = SUCCESFULL_TRIGGER;
        interrupts();    
#if ULTRASONIC_DEBUGGING == true
        Serial.print(this->name);
        Serial.println(" ULTRASONIC_ACQUISITION_REQUEST_SUCCESFULL ");
#endif // ULTRASONIC_DEBUGGING == true
      break;
    }
    case ULTRASONIC_ACQUISITION_TRIGGERED:{
        // request set to fast
#if ULTRASONIC_DEBUGGING == true
        Serial.print(this->name);
        Serial.println(" ACQUISITION_TRIGGERED");
#endif // ULTRASONIC_DEBUGGING == true
        retval = SENSOR_NOT_TRIGGERED;
      break;
    }
    case ULTRASONIC_ACQUISITION_ECHO_HIGH:{
      // last roud of acquisition is not done request dropped
#if ULTRASONIC_DEBUGGING == true
        Serial.print(this->name);
        Serial.println(" ACQUISITION_ECHO_HIGH");
#endif // ULTRASONIC_DEBUGGING == true
        retval = PING_OUT_OF_RANGE;
        break;
    }
      case ULTRASONIC_ACQUISITION_DONE:{
      // acquired values were not collected before initiating a new request
#if ULTRASONIC_DEBUGGING == true
        Serial.print(this->name);
        Serial.println(" ACQUISITION_DONE");
#endif // ULTRASONIC_DEBUGGING == true
        retval = RESULTS_NOT_SAMPLED;
        break;
    }
     case ULTRASONIC_OFF:{
      // driver was not initialised 
#if ULTRASONIC_DEBUGGING == true
        Serial.print(this->name);
        Serial.println(" OFF");
#endif // ULTRASONIC_DEBUGGING == true
        retval = DRIVER_OFF;
        break;
    }

  }
  // further acquisition is done from interrupt context
  return retval;
}
