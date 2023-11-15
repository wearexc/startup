#include "stm32f10x.h"                  // Device header
#include "W25Q64_Ins.h"

void W25Q64_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void W25Q64_Start(void)
{
	W25Q64_W_SS(0);
}

void W25Q64_Stop(void)
{
	W25Q64_W_SS(1);
}



void W25Q64_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	W25Q64_W_SS(1);
}

uint8_t W25Q64_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	
	SPI_I2S_SendData(SPI1, ByteSend);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI1);
}


void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_JEDEC_ID);
	*MID = W25Q64_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = W25Q64_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= W25Q64_SwapByte(W25Q64_DUMMY_BYTE);
	W25Q64_Stop();
}

void W25Q64_WriteEnable(void)
{
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_WRITE_ENABLE);
	W25Q64_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((W25Q64_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	W25Q64_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_PAGE_PROGRAM);
	W25Q64_SwapByte(Address >> 16);
	W25Q64_SwapByte(Address >> 8);
	W25Q64_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		W25Q64_SwapByte(DataArray[i]);
	}
	W25Q64_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	W25Q64_SwapByte(Address >> 16);
	W25Q64_SwapByte(Address >> 8);
	W25Q64_SwapByte(Address);
	W25Q64_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	W25Q64_Start();
	W25Q64_SwapByte(W25Q64_READ_DATA);
	W25Q64_SwapByte(Address >> 16);
	W25Q64_SwapByte(Address >> 8);
	W25Q64_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = W25Q64_SwapByte(W25Q64_DUMMY_BYTE);
	}
	W25Q64_Stop();
}
