#include "Global_variable_defs.h"
#include "src/Autonomous driving/Autonomous_driving.h"
#include "src/I2C Slave/I2c_slave.h"
#include "src/Monitoring/Monitoring.h"


//Relays
Relay Cooling_fan{Cooling_fan_pin, "Cooling fan"};

//Battery
Battery Battery_pack{Voltage_sensor_pin};
//vacuum Drum
Tub Vacuum_tub {Drum_sensor_pin};


//Ultrasonic sensors
Ultrasonic ult_stanga("stanga", Ultrasonic_stanga_trigger_pin,Ultrasonic_stanga_echo_pin,ult_stanga_read);
Ultrasonic ult_fata("fata",Ultrasonic_fata_trigger_pin,Ultrasonic_fata_echo_pin,ult_fata_read);
Ultrasonic ult_dreapta("dreapta",Ultrasonic_dreapta_trigger_pin,Ultrasonic_dreapta_echo_pin,ult_dreapta_read);
//H_bridge controll
Hbridge Left_motor{H_bridge_ena,H_bridge_in1,H_bridge_in2};
Hbridge Right_motor{H_bridge_enb,H_bridge_in3,H_bridge_in4};
//Powertrain
Powertrain Powertrain{&Left_motor, &Right_motor};
// Monitoring unit
Monit Monitoring_unit{Shutoff_pin, &Cooling_fan,&Battery_pack,&Vacuum_tub};

//Autonomous driving
Autodrv Autonomous_driving{&ult_stanga,&ult_fata,&ult_dreapta,&Powertrain};

//I2c_slave
I2c_slave Arduino_mega{ADDRESS_I2C,RxCallback,TxCallback};


void setup() {
  Monitoring_unit.Init();

  Serial.begin(115200);
  delay(200);
  Arduino_mega.Init();
  Autonomous_driving.Autodrv_Init();

}

void loop() {
  CurrMs_10ms = millis();
  CurrMs_100ms = millis();
    
    if (Task_10ms) {
      Task_10ms_pushforward;
      Autonomous_driving.Autodrv_MainTask();
    }

    if (Task_100ms) {
      Task_100ms_pushforward;
      Monitoring_unit.Assert_state();
      Monitoring_unit.Process_requests();
    }

}
