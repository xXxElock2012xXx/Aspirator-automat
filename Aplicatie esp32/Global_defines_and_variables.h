#ifndef GLOBAL_DEFINES_AND_VARIABLES
#define GLOBAL_DEFINES_AND_VARIABLES


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define ARDUINO_SLAVE_ADDRESS 0X08



unsigned long long int  CurrMs_1000ms;
unsigned long long int  PrevMs_1000ms = 0;
unsigned long long int  CurrMs_10000ms;
unsigned long long int  PrevMs_10000ms = 0;
unsigned long long int  CurrMs_100ms;
unsigned long long int  PrevMs_100ms = 0;

#define Task_100ms (CurrMs_100ms - PrevMs_100ms >= 100)
#define Task_1000ms (CurrMs_1000ms - PrevMs_1000ms >= 1000)
#define Task_10000ms (CurrMs_10000ms - PrevMs_10000ms >= 10000)
#define Task_100ms_pushforward (PrevMs_100ms = CurrMs_100ms)
#define Task_1000ms_pushforward (PrevMs_1000ms = CurrMs_1000ms)
#define Task_10000ms_pushforward (PrevMs_10000ms = CurrMs_10000ms)


char sensors[3][20] = {"Stanga", "Fata", "Dreapta"};
char directions[6][20] = {"first forward", "forward", "first left", "left", "first right", "right"};
uint8_t location_data[3][3];
uint8_t filtered_measurements[3];
uint8_t last_movement_decision;
uint8_t drum_state;
float battery_voltage_buf;
int battery_percentage_buf = 100;
float temperature;

#define BLYNK_TEMPLATE_ID "TMPL4sPnlTN7N"
#define BLYNK_TEMPLATE_NAME "Aspirator"
#define BLYNK_AUTH_TOKEN "c4TEk717ZeOKyOS9l-YL8YGiduS_8Elr"

#define USERNAME "Chip Pfizer"//"DIGI-xC6R"
#define PASSWORD "bere rece"//"sQddFfKdXk"




#endif