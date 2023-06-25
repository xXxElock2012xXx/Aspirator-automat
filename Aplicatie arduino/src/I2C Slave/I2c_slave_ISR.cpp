#include "I2c_slave.h"
#include "I2c_slave_ISR.h"

extern I2c_slave Arduino_mega;
extern uint8_t recieved_messages_buffer;

// Function that executes whenever data is requested by master.
void TxCallback (void) {
  
  Arduino_mega.Compile_message();

  if (Arduino_mega.Get_state() == I2C_MESSAGE_COMPILED){
    Arduino_mega.Set_status(Arduino_mega.g_twi.SlaveQueueNonBlocking(Arduino_mega.Get_message(), Arduino_mega.Get_message_size()));
    // message variable needs to be cleared
    Arduino_mega.Clear_message();
    Arduino_mega.Set_request(0u); // work done
    Arduino_mega.Set_state(I2C_SLAVE_IDLE);
  }else {
    //Serial.println("message was not compiled properly before sending");
  }

}

// Function that executes whenever data is received from master.
void RxCallback(const uint8_t data[], const uint8_t length){
  // should only recieve 1 byte 
  Arduino_mega.Set_request(data[0]);
  uint8_t request = Arduino_mega.Get_request(); 
  
  if((request & 0xF0) != 0){ // if a command is to be executed;

    recieved_messages_buffer = request; // copy to global buffer
    Arduino_mega.Set_request(0u); // work done
    
  }else{ // data is to be sent

    Arduino_mega.Set_state((enum I2c_slave_state)request);

  }

}








