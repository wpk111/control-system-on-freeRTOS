#include "myiicg.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_InitG(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}
//����IIC��ʼ�ź�
void IIC_StartG(void)
{
	SDA_OUTG();     //sda�����
	IIC_SDAG=1;	  	  
	IIC_SCLG=1;
	delay_us(4);
 	IIC_SDAG=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCLG=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_StopG(void)
{
	SDA_OUTG();//sda�����
	IIC_SCLG=0;
	IIC_SDAG=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCLG=1; 
	IIC_SDAG=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_AckG(void)
{
	u8 ucErrTime=0;
	SDA_ING();      //SDA����Ϊ����  
	IIC_SDAG=1;delay_us(1);	   
	IIC_SCLG=1;delay_us(1);	 
	while(READ_SDAG)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_StopG();
			return 1;
		}
	}
	IIC_SCLG=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_AckG(void)
{
	IIC_SCLG=0;
	SDA_OUTG();
	IIC_SDAG=0;
	delay_us(2);
	IIC_SCLG=1;
	delay_us(2);
	IIC_SCLG=0;
}
//������ACKӦ��		    
void IIC_NAckG(void)
{
	IIC_SCLG=0;
	SDA_OUTG();
	IIC_SDAG=1;
	delay_us(2);
	IIC_SCLG=1;
	delay_us(2);
	IIC_SCLG=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_ByteG(u8 txd)
{                        
    u8 t;   
	  SDA_OUTG(); 	    
    IIC_SCLG=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
			{              
        IIC_SDAG=(txd&0x80)>>7;
        txd<<=1; 	  
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCLG=1;
				delay_us(2); 
				IIC_SCLG=0;	
				delay_us(2);
			}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_ByteG(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA_ING();//SDA����Ϊ����
    for(i=0;i<8;i++ )
			{
        IIC_SCLG=0; 
        delay_us(2);
		    IIC_SCLG=1;
        receive<<=1;
        if(READ_SDAG)
					receive++;   
		    delay_us(1); 
			}					 
    if (!ack)
        IIC_NAckG();//����nACK
    else
        IIC_AckG(); //����ACK   
    return receive;
}



























