#ifndef __MY_H_G
#define __MY_H_G
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
 
#define SDA_ING()  {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=(u32)8<<28;}
#define SDA_OUTG() {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCLG    PGout(6) //SCL
#define IIC_SDAG    PGout(7) //SDA	 
#define READ_SDAG   PGin(7)  //����SDA 

#define AIR_GPIO   PAout(4)  //���ÿ���

//IIC���в�������
void IIC_InitG(void);                //��ʼ��IIC��IO��				 
void IIC_StartG(void);				//����IIC��ʼ�ź�
void IIC_StopG(void);	  			//����IICֹͣ�ź�
void IIC_Send_ByteG(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_ByteG(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_AckG(void); 				//IIC�ȴ�ACK�ź�
void IIC_AckG(void);					//IIC����ACK�ź�
void IIC_NAckG(void);				//IIC������ACK�ź�

void IIC_Write_One_ByteG(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_ByteG(u8 daddr,u8 addr);	  
#endif
















