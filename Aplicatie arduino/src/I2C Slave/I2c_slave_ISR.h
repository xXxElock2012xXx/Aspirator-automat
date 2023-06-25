#ifndef I2C_SLAVE_ISR_H
#define I2C_SLAVE_ISR_H
void TxCallback(void);
void RxCallback(const uint8_t data[], const uint8_t length);
#endif