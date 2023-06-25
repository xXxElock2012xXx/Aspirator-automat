#ifndef GLOBAL_VARIABLE_DEFS_H
#define GLOBAL_VARIABLE_DEFS_H
#include <Arduino.h>

// generic defines and variables
#define time (String(millis()))
#define off 0x00
#define on 0x01

// rtos defines and variables
//application 10ms task
unsigned long long int  CurrMs_10ms;
unsigned long long int  PrevMs_10ms = 0;

#define Task_10ms (CurrMs_10ms - PrevMs_10ms >= 10)
#define Task_100ms (CurrMs_100ms - PrevMs_100ms >= 100)
#define Task_10ms_pushforward (PrevMs_10ms = CurrMs_10ms)
#define Task_100ms_pushforward (PrevMs_100ms = CurrMs_100ms)
//application 100ms task 
unsigned long long int  CurrMs_100ms;
unsigned long long int  PrevMs_100ms = 5; // delay the execution of the 100 ms task by 5, so as to not averlap with 10 ms task

//Autodrv init task variables
unsigned long long int startup_CurrMs;
unsigned long long int startup_PrevMs;

//I2c_slave variables
enum address_i2c_t : byte
{
    ADDRESS_I2C = 0x08 // Address of Arduino mega
};



//Ultrasonic defines and variables
#define Ultrasonic_sample_no 3
#define Ultrasonic_stanga_trigger_pin 27
#define Ultrasonic_fata_trigger_pin 39
#define Ultrasonic_dreapta_trigger_pin 51
#define Ultrasonic_stanga_echo_pin 3
#define Ultrasonic_fata_echo_pin 2
#define Ultrasonic_dreapta_echo_pin 18

//Hbridge defines and variables
#define H_bridge_ena 4
#define H_bridge_in1 24
#define H_bridge_in2 22
#define H_bridge_enb 5
#define H_bridge_in3 25
#define H_bridge_in4 23
//Autodrv defines and variables
//Global buffers for debugging
uint8_t location_data_buffer[3][3];
uint8_t filtered_measurements_buffer[3];
uint8_t last_movement_decision_buffer;
//Relay defines and variables

#define Cooling_fan_pin 31
#define Drum_fan_pin 53 // deprecated

// Battery defines and variables

#define Voltage_sensor_pin A0


// Vacuum defines and variables

#define Drum_sensor_pin A1

// Monitoring unit defines and variables

#define Shutoff_pin 13

uint8_t recieved_messages_buffer;

float battery_voltage_buffer;

uint8_t drum_status_buffer;

float temperature_buffer; // deprecated

#endif