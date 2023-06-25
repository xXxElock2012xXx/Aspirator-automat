#include "Global_defines_and_variables.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "I2c_Master.h"
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

I2cMaster Esp32{&display, &htu, ARDUINO_SLAVE_ADDRESS};

void Send_packet(){

  Blynk.virtualWrite(V2,battery_percentage_buf);
  Blynk.virtualWrite(V3,temperature);
  Blynk.run();


}
void setup() {

  Serial.begin(9600);
  delay(1000);
  Blynk.begin(BLYNK_AUTH_TOKEN, "DIGI-xC6R","sQddFfKdXk");
  Esp32.Init();
  
}
/*
location_data[3][3];
filtered_measurements[3];
last_movement_decision;
drum_state;
*/



void loop() {
  CurrMs_100ms = millis();
  CurrMs_1000ms = millis();
  CurrMs_10000ms = millis();

    if (Task_100ms) {
      Task_100ms_pushforward;
      Esp32.Send_command(DISTANCE_DATA_REQUEST);
      Esp32.Send_command(BATTERY_DATA_REQUEST);
      Esp32.Send_command(DRUM_DATA_REQUEST);
      Esp32.Send_command(TEMPERATURE_DATA_REQUEST);
      Esp32.Process_data();
    }
    if (Task_1000ms) {
      Task_1000ms_pushforward;
      Blynk.virtualWrite(V3,temperature);
    }
    if (Task_10000ms) {
      Task_10000ms_pushforward;
      Blynk.virtualWrite(V2,battery_percentage_buf);
    }
  

}

BLYNK_WRITE(V1){
  Blynk.logEvent("aspirator_oprit");
  Serial.println("EVENT BLYNK");
  Blynk.virtualWrite(V3,0);
  Blynk.virtualWrite(V2,0);
  Esp32.Send_command(SYSTEM_SHUTDOWN_REQUEST);

}