#include "Autonomous_driving.h"
extern uint8_t location_data_buffer[3][3];
extern uint8_t filtered_measurements_buffer[3];
extern uint8_t last_movement_decision_buffer;

Autodrv::Autodrv(){}

Autodrv::Autodrv(Ultrasonic* sensor1, Ultrasonic* sensor2, Ultrasonic* sensor3, Powertrain* powertrain){

  this->ult_left = sensor1;
  this->ult_front = sensor2;
  this->ult_right = sensor3;
  this->Pwrtrain = powertrain;
  this->state = AUTODRV_OFF;
}

void Autodrv::Autodrv_MainTask(){
  
  switch(current_step){
    case 0:{
      this->state = AUTODRV_MEASURING_LEFT;
      this->ult_left->Trigger();
      break;
    }
    case 1:{
      this->sampling_matrix[0][0] = this->ult_left->Get_measured_distance();
      this->ult_left->Trigeger();
      break;
    }
    case 2:{
      this->sampling_matrix[0][1] = this->ult_left->Get_measured_distance();
      this->ult_left->Trigger();
      break;
    }
    case 3:{
      this->state = AUTODRV_MEASURING_FRONT;
      this->sampling_matrix[0][2] = this->ult_left->Get_measured_distance();
      this->ult_front->Trigger();
      break;
    }
    case 4:{
      this->sampling_matrix[1][0] = this->ult_front->Get_measured_distance();
      this->ult_front->Trigger();
      break;
    }
    case 5:{
      this->sampling_matrix[1][1] = this->ult_front->Get_measured_distance();
      this->ult_front->Trigger();
      break;
    }
    case 6:{
      this->state = AUTODRV_MEASURING_RIGHT;
      this->sampling_matrix[1][2] = this->ult_front->Get_measured_distance();
      this->ult_right->Trigger();
      break;
    }
    case 7:{
      this->sampling_matrix[2][0] = this->ult_right->Get_measured_distance();
      this->ult_right->Trigger();
      break;
    }
    case 8:{
      this->sampling_matrix[2][1] = this->ult_right->Get_measured_distance();
      this->ult_right->Trigger();
      break;
    }
    case 9:{
      this->sampling_matrix[2][2] = this->ult_right->Get_measured_distance();
      this->state = AUTODRV_FILTERING_AND_STEERING;
      for (uint8_t it = 0 ; it < 3 ; it++){
        this->location_data[it][this->sampling_column] = 255;
      }
      //acquisition done, start taking medians 
      uint16_t sum = 0;
      uint8_t count = 0;
      for (uint8_t it = 0 ; it < 3 ; it++){
        sum = 0;
        count = 0;
        for (uint8_t jt = 0 ; jt < 3 ; jt++){
          if (this->sampling_matrix[it][jt] != 255){
            sum+=this->sampling_matrix[it][jt];
            count++;
          }
        }
        if (count)
          //this->filtered_measurements[it] = sum/count;
          this->location_data[it][this->sampling_column] = sum/count;
      }
    
      //averaging samples done, start averaging part 2
      for (uint8_t it = 0 ; it < 3 ; it++){
        sum = 0;
        count = 0;
        for (uint8_t jt = 0 ; jt < 3 ; jt++){
          sum+=this->location_data[it][jt];
          count++;
        }
          this->filtered_measurements[it] = sum/count;
      }
      //filtering done start moving
      if (Front_distance < Minimum_allowed_distance){ // if you have something in front of you
        // calculate the best trajectory and go
        if (Left_distance - Right_distance >= 0 ){ 
          //go left
          this->Autodrv_Move(Powertrain_left);
        }else{
          //go right
          this->Autodrv_Move(Powertrain_right);
        }                
      }else { // you do not have something in front of you but you may be closing in on a wall
        if (Left_distance < Minimum_allowed_distance ){
          //go right
          this->Autodrv_Move(Powertrain_right);
        }else if (Right_distance < Minimum_allowed_distance ) {
          //go left
          this->Autodrv_Move(Powertrain_left); 
        }else{
          //go forwards
          this->Autodrv_Move(Powertrain_forward);
 
        }
      }
  #if Autodrv_debug == true
        Serial.println(this->last_movement_decision);
  #endif //Autrodrv_debug == true
        this->Autodrv_Export_Filtered_Measurements();
        this->Autodrv_Export_Location_Data();
        this->Autodrv_Export_last_movement_decision();
      break;
    }
  }

  current_step ++;
  current_step%=10;
  this->sampling_column++;
  this->sampling_column%=3;

}

void Autodrv::Autodrv_Init(){
  noInterrupts();
  this->state = AUTODRV_INIT;
  this->ult_left->Init();
  this->ult_front->Init();
  this->ult_right->Init();
  this->Pwrtrain->Init();
  interrupts();
  this->sampling_column = 0;

   for (uint8_t it = 0 ; it < 3 ; it++){
    for (uint8_t jt = 0 ; jt < 3 ; jt++){
      this->location_data[it][jt] = 255;
    }
   }
  while(this->sampling_column < 3){
    while (1){ // temporary os for init
      startup_CurrMs = millis();
      if (startup_CurrMs - startup_PrevMs >= 10) {
        startup_PrevMs = startup_CurrMs;
        switch(current_step){
          case 0:{
            this->ult_left->Trigger();
            break;
          }
          case 1:{
            this->sampling_matrix[0][0] = this->ult_left->Get_measured_distance();
            this->ult_left->Trigger();
            break;
          }
          case 2:{
            this->sampling_matrix[0][1] = this->ult_left->Get_measured_distance();
            this->ult_left->Trigger();
            break;
          }
          case 3:{
            this->sampling_matrix[0][2] = this->ult_left->Get_measured_distance();
            this->ult_front->Trigger();
            break;
          }
          case 4:{
            this->sampling_matrix[1][0] = this->ult_front->Get_measured_distance();
            this->ult_front->Trigger();
            break;
          }
          case 5:{
            this->sampling_matrix[1][1] = this->ult_front->Get_measured_distance();
            this->ult_front->Trigger();
            break;
          }
          case 6:{
            this->sampling_matrix[1][2] = this->ult_front->Get_measured_distance();
            this->ult_right->Trigger();
            break;

          }
          case 7:{
            this->sampling_matrix[2][0] = this->ult_right->Get_measured_distance();
            this->ult_right->Trigger();
            break;
          }
          case 8:{
            this->sampling_matrix[2][1] = this->ult_right->Get_measured_distance();
            this->ult_right->Trigger();
            break;
          }
          case 9:{
            this->sampling_matrix[2][2] = this->ult_right->Get_measured_distance();
            //acquisition done, start taking medians 
            uint16_t sum = 0;
            uint8_t count = 0;
            for (uint8_t it = 0 ; it < 3 ; it++){
              sum = 0;
              count = 0;
              for (uint8_t jt = 0 ; jt < 3 ; jt++){
                if (this->sampling_matrix[it][jt] != 255){
                  sum+=this->sampling_matrix[it][jt];
                  count++;
                }
              }
              if (count)
                this->location_data[it][this->sampling_column] = sum/count;
            }
            // filtering done start moving
            if (this->location_data[1][0] < Minimum_allowed_distance){ // if you have something in front of you
              // calculate the best trajectory and go
              if (this->location_data[0][0] - this->location_data[2][0] < 0 ){ 
                //go right
                this->Pwrtrain->Fixed_turn(Powertrain_right,100u);
                this->last_movement_decision = AUTODRV_FIRST_RIGHT; 
              }else{
                //go left
                this->Pwrtrain->Fixed_turn(Powertrain_left, 100u);
                this->last_movement_decision = AUTODRV_FIRST_LEFT; 
              }                
            }else {
              if (this->location_data[0][0] < Minimum_allowed_distance ){
                //go right
                this->Pwrtrain->Fixed_turn(Powertrain_right,100u);
                this->last_movement_decision = AUTODRV_FIRST_RIGHT; 
              }else if (this->location_data[0][0] < Minimum_allowed_distance ) {
                //go left
                this->Pwrtrain->Fixed_turn(Powertrain_left, 100u);
                this->last_movement_decision = AUTODRV_FIRST_LEFT; 
              }else{
              //go forwards
              this->Pwrtrain->Move_forward(100u);
              this->last_movement_decision = AUTODRV_FIRST_FORWARD; 
              }
            }
            break;
          
          }
        }
        current_step ++;
        if(current_step == 10){ // exit first stage of init
          current_step = 0;
          this->sampling_column++;
  #if Autodrv_debug == true
          Serial.println(this->last_movement_decision);
  #endif //Autrodrv_debug == true
          break;
        }
      }
    }
  }
  this->sampling_column = 0;
}

void Autodrv::Autodrv_Export_Location_Data(){
  if(this->state == AUTODRV_FILTERING_AND_STEERING)
  for (uint8_t it = 0 ; it < 3 ; it++){
    for (uint8_t jt = 0 ; jt < 3 ; jt++){
      location_data_buffer[it][jt] = this->location_data[it][jt];
    }
    
  } 
}

void Autodrv::Autodrv_Export_Filtered_Measurements(){

  if(this->state == AUTODRV_FILTERING_AND_STEERING)
  for (uint8_t it = 0 ; it < 3 ; it++){
    filtered_measurements_buffer[it] = this->filtered_measurements[it];
  }
}

void Autodrv::Autodrv_Export_last_movement_decision(){
  if(this->state == AUTODRV_FILTERING_AND_STEERING)
  last_movement_decision_buffer = this->last_movement_decision;
}

void Autodrv::Autodrv_Move(PwrTraind_Direction direction){
  enum Autodrv_loc_state last_decision = this->last_movement_decision;
  switch(direction){
    case Powertrain_left:{
      //go left
      if(last_decision == AUTODRV_FIRST_LEFT){
        //slow down a little
        this->Pwrtrain->Fixed_turn(Powertrain_left,130u);
        this->last_movement_decision = AUTODRV_LEFT;
      }else if (last_decision != AUTODRV_LEFT){
        // go faster initially to break inertia
        this->Pwrtrain->Fixed_turn(Powertrain_left, 130u);
        this->last_movement_decision = AUTODRV_FIRST_LEFT;
      } 
      break;
    }
    case Powertrain_forward:{
      //go forwards
      if(last_decision == AUTODRV_FIRST_FORWARD){
        //slow down a little
        this->Pwrtrain->Move_forward(130u);
        this->last_movement_decision = AUTODRV_FORWARD;
      }else if (last_decision != AUTODRV_FORWARD){
        // go faster initially to break inertia
        this->Pwrtrain->Move_forward(130u);
        this->last_movement_decision = AUTODRV_FIRST_FORWARD;
      } 
      break;
    }
    case Powertrain_right:{
      //go right
      if(last_decision == AUTODRV_FIRST_RIGHT){
        //slow down a little
        this->Pwrtrain->Fixed_turn(Powertrain_right,130u);
        this->last_movement_decision = AUTODRV_RIGHT;
      }else if (last_decision != AUTODRV_RIGHT){
        // go faster initially to break inertia
        this->Pwrtrain->Fixed_turn(Powertrain_right, 130u);
        this->last_movement_decision = AUTODRV_FIRST_RIGHT;
      } 
      break;
      case Powertrain_no_direction:{
        // you done goofed
  #if Autodrv_debug == true
        Serial.println("Wrong parameter call in Autodrv_Move");
  #endif //Autrodrv_debug == true
      }
    }
  }


}



