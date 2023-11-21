#include "stm32f10x.h"                  // Device header
#include "NRF24L01.h"
#include "nRF24L01.h"
#include "nRF24L01_Ins.h"
#include "Delay.h"

const uint8_t TX_ADDRESS[32]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[32]={0x34,0x43,0x10,0x10,0x01}; //发送地址
uint8_t NRF24L01_Write_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count);
uint8_t NRF24L01_Read_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count);

//SPI2配置
void NRF24L01_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)BitValue);
}

void NRF24L01_W_CE(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)BitValue);
}

void NRF24L01_W_CSN(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_9, (BitAction)BitValue);
}

uint8_t NRF24L01_IQR()
{
	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10);
}

void NRF24L01_Start(void)
{
	NRF24L01_W_SS(0);
}

void NRF24L01_Stop(void)
{
	NRF24L01_W_SS(1);
}

uint8_t NRF24L01_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);  
	
	SPI_I2S_SendData(SPI2, ByteSend);
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI2);
}

//NRF24L01初始化
void NRF24L01_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
	
	NRF24L01_W_SS(1);
	
    NRF24L01_W_CE(0); //使能 24L01
	NRF24L01_W_CSN(1); //SPI 片选取消
}

//检查本机是否连接上设备
uint8_t NRF24L01_Check(void)
{
	uint8_t temp = 1,i,buf[5] = {0x66,0x66,0x66,0x66,0x66};
	NRF24L01_Write_Buf(NRP24L01_W_REGISTER+NRP24L01_TX_ADDR,buf,5);
	NRF24L01_Read_Buf(NRP24L01_TX_ADDR,buf,5);
	for(i = 0;i < 5;i++)
	{
		if(buf[i] != 0x66)
		{
			temp = 0;
			break;
		}
	}
	return temp;
}
//指定地址写寄存器
uint8_t NRF24L01_Write_Reg(uint8_t Address,uint8_t DataAddress)
{
	uint8_t num;
	NRF24L01_W_CSN(0);
	num = NRF24L01_SwapByte(Address);
	NRF24L01_SwapByte(DataAddress);
	NRF24L01_W_CSN(1);
	return num;
}
//指定地址读寄存器
uint8_t NRF24L01_Read_Reg(uint8_t Address)
{
	uint8_t DataAddress;
	NRF24L01_W_CSN(0);
	NRF24L01_SwapByte(Address);
	DataAddress = NRF24L01_SwapByte(0xff);
	NRF24L01_W_CSN(1);
	return DataAddress;
}
//指定地址写数据
uint8_t NRF24L01_Write_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count)
{
	uint8_t Data,i;
	NRF24L01_W_CSN(0);
	Data = NRF24L01_SwapByte(Address);
	for(i = 0;i < Count;i++)
	{
		NRF24L01_SwapByte(*DataArray++);
	}
	NRF24L01_W_CSN(1);
	return Data;
}
//指定地址读数据
uint8_t NRF24L01_Read_Buf(uint8_t Address,uint8_t *DataArray,uint8_t Count)
{
	uint8_t Data,i;
	NRF24L01_W_CSN(0);
	Data = NRF24L01_SwapByte(Address);
	for(i = 0;i < Count;i++)
	{
		DataArray[i] = NRF24L01_SwapByte(0xff);
	}
	NRF24L01_W_CSN(1);
	return Data;
}

uint8_t NRF24L01_TxPacket(uint8_t *TxData)
{
	uint8_t Data;
	NRF24L01_W_CE(0);
	NRF24L01_Write_Buf(NRP24L01_W_TX_PAYLOAD,TxData,32);
	NRF24L01_W_CE(1);
	Delay_us(10);
	while(NRF24L01_IQR() != 0);
	Data = NRF24L01_Read_Reg(NRP24L01_STATUS);
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER + NRP24L01_STATUS,Data);
	if(Data & NRP24L01_MAX_TX)
	{
		NRF24L01_Write_Reg(NRP24L01_FLUSH_TX,0xff);
		return NRP24L01_MAX_TX;
	}
	if(Data & NRP24L01_TX_OK)
	{
		return NRP24L01_TX_OK;
	}
	return 0xff;
}

uint8_t NRF24L01_RxPacket(uint8_t *RxData)   /////////////////////////////////////
{
	uint8_t Data,Length;
	Length = sizeof(RxData) / sizeof(RxData [0]);
	Data = NRF24L01_Read_Reg(NRP24L01_STATUS);
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER + NRP24L01_STATUS,Data);
	if(Data & NRP24L01_RX_OK)
	{
		NRF24L01_Read_Buf(NRP24L01_R_RX_PAYLOAD,RxData,Length);
		NRF24L01_Write_Reg(NRP24L01_FLUSH_RX,0xff);
		return 1;
	}
	return 0;
}

void NRF24L01_Rx_Mode(void)
{
	NRF24L01_W_CE(0); 
	NRF24L01_Write_Buf(NRP24L01_W_REGISTER+NRP24L01_RX_ADDR_P0 ,(uint8_t *)RX_ADDRESS,32); //写RX节点地址,32为传输数据宽度
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_EN_AA,0x01);       //使能通道0的自动应答
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_EN_RXADDR,0x01);   //使能通道0的接收地址
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_RF_CH,40);         //使能通道0的接收地址
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_RX_PW_P0,32);      //选择通道0的有效数据
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_RF_SETUP,0x0f);    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	
	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_CONFIG,0x0f);  //配置基本工作模式的参数;接收模式
	NRF24L01_W_CE(1); 
}


void NRF24L01_Tx_Mode(void)
{														 
	NRF24L01_W_CE(0); 
  	NRF24L01_Write_Buf(NRP24L01_W_REGISTER+NRP24L01_TX_ADDR,(u8*)TX_ADDRESS,32);//写TX节点地址 
  	NRF24L01_Write_Buf(NRP24L01_W_REGISTER+NRP24L01_RX_ADDR_P0,(u8*)RX_ADDRESS,32);   
 
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_SETUP_RETR,0x1a);
 
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_RF_CH,40);       //设置RF通道为40
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_RF_SETUP,0x0f);  
   
  	NRF24L01_Write_Reg(NRP24L01_W_REGISTER+NRP24L01_CONFIG,0x0e);    
 
	NRF24L01_W_CE(1);
}
