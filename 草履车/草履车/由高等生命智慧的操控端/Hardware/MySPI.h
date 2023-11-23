#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(void);
void NRF24L01_W_CE(uint8_t BitValue);
void NRF24L01_W_CSN(uint8_t BitValue);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);
uint8_t NRF24L01_IQR();
#endif
