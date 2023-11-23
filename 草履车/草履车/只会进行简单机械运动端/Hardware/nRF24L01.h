#ifndef __NRF24L01_H
#define __NRF24L01_H

void NRF24L01_Init(void);
void NRF24L01_Tx_Mode(void);
void NRF24L01_Rx_Mode(void);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_Write_Reg(uint8_t Address,uint8_t DataAddress);
uint8_t NRF24L01_Read_Reg(uint8_t Address);
uint8_t NRF24L01_Write_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count);
uint8_t NRF24L01_Read_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count);
uint8_t NRF24L01_TxPacket(uint8_t *TxData);
uint8_t NRF24L01_RxPacket(uint8_t *RxData);
#endif
