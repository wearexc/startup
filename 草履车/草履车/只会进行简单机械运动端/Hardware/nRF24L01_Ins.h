#ifndef __NRF24L01_INS_H
#define __NRF24L01_INS_H


//NRP24L01_R_REGISTER   000A AAAA   读寄存器，AAAAA是需要写入的地址。且是LSB(小端序)（比如数据：1234,存储为地址0->34,1->12。大端序为地址0->12,1->32)
//NRP24L01_W_REGISTER   001A AAAA   写寄存器，AAAAA是需要写入的地址。且是LSB
//NRP24L01_W_REGISTERW_ACK_PAYLOAD    1010 1PPP 不懂
//命令字
#define NRP24L01_R_REGISTER							0x00
#define NRP24L01_W_REGISTER							0x20
#define NRP24L01_R_RX_PAYLOAD						0x61    //读命令，用于RX模式
#define NRP24L01_W_TX_PAYLOAD						0xa0    //写命令，用于TX模式
#define NRP24L01_FLUSH_TX				            0xe1    //刷新TX FIFO，用于TX模式
#define NRP24L01_FLUSH_RX				            0xe2    //刷新RX FIFO，用于RX模式
#define NRP24L01_REUSE_TX_PL				        0xe3    //重新使用上一包数据,CE为高,数据包被不断发送
#define NRP24L01_ACTIVATE							0x50    //类似于隐藏功能可用于写入/读取FIFO寄存器的信息等
#define NRP24L01_R_RX_PL_WID				    	0x60    //读取RX FIFO中顶部R_RX_payload的RX有效载荷宽度（机翻）
#define NRP24L01_W_TX_PAYLOAD_NOACK					0x58    //用于TX模式，对于特定的数据禁用ACK。
#define NRP24L01_NOP						    	0xff    //啥也没有，可能用于读取STATUS寄存器。
//寄存器地址
#define NRP24L01_CONFIG     						0x00 	//配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使 //能;bit6:中断RX_DR使能
#define NRP24L01_EN_AA      						0x01 	//使能自动应答功能  bit0~5,对应通道0~5
#define NRP24L01_EN_RXADDR  						0x02 	//接收地址允许,bit0~5,对应通道0~5
#define NRP24L01_SETUP_AW   						0x03 	//设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字 节;
#define NRP24L01_SETUP_RETR 						0x04 	//建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时250*x+86us
#define NRP24L01_RF_CH      						0x05 	//RF通道,bit6:0,工作通道频率;
#define NRP24L01_RF_SETUP   						0x06 	//RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define NRP24L01_STATUS     						0x07 	//状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最 大:6);bit4,达到最多次重发bit5:数据发送完成中断;bit6:接收数据中断;
#define NRP24L01_MAX_TX  							0x10 	//达到最大发送次数中断
#define NRP24L01_TX_OK   							0x20 	//TX发送完成中断
#define NRP24L01_RX_OK   							0x40 	//接收到数据中断
 
#define NRP24L01_OBSERVE_TX     					0x08    //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define NRP24L01_CD             					0x09    //载波检测寄存器,bit0,载波检测;
#define NRP24L01_RX_ADDR_P0     					0x0A    //数据通道0接收地址,最大长度5个字节,低字节在前
#define NRP24L01_RX_ADDR_P1     					0x0B    //数据通道1接收地址,最大长度5个字节,低字节在前
#define NRP24L01_RX_ADDR_P2      					0x0C 	//数据通道2接收地址,最低字节可设置,高字节,必须同 RX_ADDR_P1[39:8]相等;
#define NRP24L01_RX_ADDR_P3      					0x0D 	//数据通道3接收地址,最低字节可设置,高字节,必须同 RX_ADDR_P1[39:8]相等;
#define NRP24L01_RX_ADDR_P4      					0x0E 	//数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define NRP24L01_RX_ADDR_P5      					0x0F 	//数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
						
#define NRP24L01_TX_ADDR         					0x10 	//发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define NRP24L01_RX_PW_P0        					0x11 	//接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_RX_PW_P1        					0x12 	//接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_RX_PW_P2        					0x13 	//接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_RX_PW_P3        					0x14 	//接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_RX_PW_P4        					0x15 	//接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_RX_PW_P5        					0x16 	//接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRP24L01_NRF_FIFO_STATUS 					0x17 	//FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一 数据包.0,不循环;



#endif
