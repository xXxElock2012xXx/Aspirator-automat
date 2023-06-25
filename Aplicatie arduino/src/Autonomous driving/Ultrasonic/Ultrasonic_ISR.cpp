#include "Ultrasonic.h"
extern Ultrasonic ult_stanga;
extern Ultrasonic ult_dreapta;
extern Ultrasonic ult_fata;

void ult_stanga_read(){
  switch(ult_stanga.Get_state()){
    case ULTRASONIC_ACQUISITION_TRIGGERED:{

      ult_stanga.Set_t1((unsigned long long int)micros());
      ult_stanga.Set_state(ULTRASONIC_ACQUISITION_ECHO_HIGH);
#if ISR_DEBUGGING == true 
      Serial.print(ult_stanga.name);
      Serial.print(" ");
      Serial.println(" __ISR__ " + String(micros()) + " ACQUISITION_ECHO_HIGH ");
#endif // ISR_DEBUGGING == true 
      break;
    }
    case ULTRASONIC_ACQUISITION_ECHO_HIGH:{
      
      ult_stanga.Set_t2((unsigned long long int)micros());
      ult_stanga.Set_state(ULTRASONIC_ACQUISITION_DONE);
#if ISR_DEBUGGING == true
      Serial.print(ult_stanga.name);
      Serial.print(" ");
      Serial.println(" __ISR__ " + String(micros()) +" ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
      ult_stanga.Set_measured_distance ((ult_stanga.Get_t2() - ult_stanga.Get_t1()) * ult_stanga.Get_sound_speed());
      break;
      
    }
      case ULTRASONIC_IDLE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_stanga.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_IDLE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_ACQUISITION_DONE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_stanga.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_OFF:{
#if ISR_DEBUGGING == true 
      Serial.print(ult_stanga.name);
      Serial.print(" ");
      Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_OFF ");
#endif // ISR_DEBUGGING == true 
      break;
      }
  }
}
void ult_fata_read() {
 switch(ult_fata.Get_state()){
    case ULTRASONIC_ACQUISITION_TRIGGERED:{

      ult_fata.Set_t1((unsigned long long int)micros());
      ult_fata.Set_state(ULTRASONIC_ACQUISITION_ECHO_HIGH);
#if ISR_DEBUGGING == true 
      Serial.print(ult_fata.name);
      Serial.print(" ");
      Serial.println(" __ISR__ " +String(micros()) + " ACQUISITION_ECHO_HIGH ");
#endif // ISR_DEBUGGING == true 
      break;
    }
    case ULTRASONIC_ACQUISITION_ECHO_HIGH:{
      
      ult_fata.Set_t2((unsigned long long int)micros());
      ult_fata.Set_state(ULTRASONIC_ACQUISITION_DONE);
#if ISR_DEBUGGING == true 
      Serial.print(ult_fata.name);
      Serial.print(" ");
      Serial.println(" __ISR__ " +String(micros()) +" ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
      ult_fata.Set_measured_distance ((ult_fata.Get_t2() - ult_fata.Get_t1()) * ult_fata.Get_sound_speed());
      break;
    }
          case ULTRASONIC_IDLE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_fata.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_IDLE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_ACQUISITION_DONE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_fata.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_OFF:{
#if ISR_DEBUGGING == true 
      Serial.print(ult_fata.name);
      Serial.print(" ");
      Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_OFF ");
#endif // ISR_DEBUGGING == true 
      break;
      }
  }



}
void ult_dreapta_read() {
 switch(ult_dreapta.Get_state()){
    case ULTRASONIC_ACQUISITION_TRIGGERED:{

      ult_dreapta.Set_t1((unsigned long long int)micros());
      ult_dreapta.Set_state(ULTRASONIC_ACQUISITION_ECHO_HIGH);
#if ISR_DEBUGGING == true 
      Serial.print(ult_dreapta.name);
      Serial.print(" ");
      Serial.println(" __ISR__ " + String(micros()) + " ACQUISITION_ECHO_HIGH ");
#endif // ISR_DEBUGGING == true 
      break;
    }
    case ULTRASONIC_ACQUISITION_ECHO_HIGH:{
      
      ult_dreapta.Set_t2((unsigned long long int)micros());
      ult_dreapta.Set_state(ULTRASONIC_ACQUISITION_DONE);
#if ISR_DEBUGGING == true 
      Serial.print(ult_dreapta.name);
      Serial.print(" ");
      Serial.println( " __ISR__ " + String(micros()) +" ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
      ult_dreapta.Set_measured_distance ((ult_dreapta.Get_t2() - ult_dreapta.Get_t1()) * ult_dreapta.Get_sound_speed());
      break;
    }
      case ULTRASONIC_IDLE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_dreapta.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_IDLE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_ACQUISITION_DONE:{
#if ISR_DEBUGGING == true 
        Serial.print(ult_dreapta.name);
        Serial.print(" ");
        Serial.println( " __ISR__ " + String(micros()) +" ULTRASONICIC_ACQUISITION_DONE ");
#endif // ISR_DEBUGGING == true 
        break;
      }
      case ULTRASONIC_OFF:{
#if ISR_DEBUGGING == true 
      Serial.print(ult_dreapta.name);
      Serial.print(" ");
      Serial.println( " __ISR__ " + String(micros()) +" ULTRASONIC_OFF ");
#endif // ISR_DEBUGGING == true 
      break;
      }
  }


}