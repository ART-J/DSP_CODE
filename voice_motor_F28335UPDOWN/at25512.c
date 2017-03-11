#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define  uint  unsigned int
#define  uchar unsigned char

#define  CS_0  GpioDataRegs.GPBCLEAR.bit.GPIO57=1     	//片选
#define  CS_1  GpioDataRegs.GPBSET.bit.GPIO57=1

#define  SCLK_L  GpioDataRegs.GPBCLEAR.bit.GPIO56=1 	//串行时钟
#define  SCLK_H  GpioDataRegs.GPBSET.bit.GPIO56=1

#define  DIN  GpioDataRegs.GPBDAT.bit.GPIO54			//串行输入，单片机输入引脚，eeprom输出引脚（SO）
#define  DOUT  GpioDataRegs.GPBDAT.bit.GPIO55			//串行输出，单片机输出引脚，eeprom输入引脚（SI）

void AT25128_WREN(void);
uchar AT25128_RDSR(void);
void adelay(unsigned int n);
uchar AT25128_RDSR(void);
uchar ADS_Read(void);
void WriteSPI(uchar Datt);

void initspieeprom(void)
{
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;  	 // Enable pullup
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0; 	 // GPIO = GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;  	 // GPIO = output

	GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;  	 // Enable pullup
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;  	 // GPIO = GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;      // GPIO = output

	GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;  	 // Enable pullup
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;  	 // GPIO = GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;      // GPIO = output

	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 1;  	 // Disable pullup
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;  	 // GPIO = GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;      // GPIO = intput
	EDIS;
}

void adelay(unsigned int n)
{
	unsigned int x;
	for(;n>0;n--)
		for(x=100;x>0;x--);		
}

//============================================================================================================ 
void AT25128_WREN(void)
{	 
	CS_0;
    WriteSPI(0x06);
    CS_1;
}/* 		
//============================================================================================================ 																								   
void AT25128_WRDI(void)
{	 
	nCSEE	=0;
    Communication_SPI(0x04);
	nCSEE	=1;
} 		  
//============================================================================================================ 																								   
void AT25128_WRSR(void)
{	 
	AT25128_WREN();
	nCSEE	=0;
    Communication_SPI(0x01);
    Communication_SPI(0x02);
	nCSEE	=1;
}*/ 	  
//============================================================================================================ 																								   
uchar AT25128_RDSR(void)
{	 
	uchar Temp=0;

	AT25128_WREN();
	CS_0;
    WriteSPI(0x05);
    Temp	=ADS_Read();
    CS_1;
	return(Temp);
} 

void WriteSPI(uchar Datt)
{
	uchar i=0;

	SCLK_H;
	DELAY_US(1);
	for(i=0;i<8;i++)
	{	
		SCLK_L;
		DELAY_US(1);
		if(Datt&0x80)
		DOUT=1;
		else
		DOUT=0;
		Datt<<=1;
		SCLK_H;
		DELAY_US(1);
	}	
}

uchar ADS_Read(void)
{
	uchar i=0;
	uchar res=0x00;

	SCLK_H;
	for(i=0;i<8;i++)
	{ 
		res = res<<1;
		SCLK_L;
		DELAY_US(1);
		if(DIN == 1)
		res |=0x001;
		else
		res&=~0x001;

		SCLK_H;
 
		DELAY_US(1);
	} 
	return res;
}	 
//============================================================================================================ 																								   
void AT25128_Write_Byte(uint Addr,uchar Data)
{	 
	uchar Temp=0;

	AT25128_WREN();
	CS_0;
    WriteSPI(0x02);
	WriteSPI(Addr/256);
	WriteSPI(Addr%256);
	WriteSPI(Data);
	CS_1;
    do
	{
		Temp=AT25128_RDSR();
	}while((Temp&0x01)!=0);
} 
/*		 	  
//============================================================================================================ 																								   
void AT25128_Write_nByte(uint16 Addr,uint8 *Data,uint16 Count)
{	 
	uint8 i;
	uint8 Temp=0;

	AT25128_WREN();
	nCSEE	=0;
    Communication_SPI(0x02);
	Communication_SPI(Addr/256);
	Communication_SPI(Addr%256); 
	for(i=0;i<Count;i++)
		Communication_SPI(*(Data+i));
	nCSEE	=1;	 
    do
	{
		Temp	=AT25128_RDSR();
	}while((Temp&0x01)!=0);
}*/	 	   	  
//============================================================================================================ 																								   
uchar AT25128_Read_Byte(uint Addr)
{	 
	uchar Temp=0;

	CS_0;
    WriteSPI(0x03);
	WriteSPI(Addr/256);
	WriteSPI(Addr%256); 
	Temp	=ADS_Read();
	CS_1;
	return(Temp);
} 	 	   	  
//============================================================================================================ 																								   
/*uint8 AT25128_Read_nByte(uint16 Addr,uint8 *Data,uint8 Count)
{	 
	uint8 i;
	uint8 Temp=0;

	nCSEE	=0;
    Communication_SPI(0x03);
	Communication_SPI(Addr/256);
	Communication_SPI(Addr%256); 
	for(i=0;i<Count;i++)
		*(Data+i)	=Communication_SPI(0x00);
	nCSEE	=1;
	return(Temp);
}  		 	  
//============================================================================================================ 																								   
void AT25128_Write_Time(uint16 Addr)
{	 
	uint8 i;
	uint8 Buffer[3];
	 
	DS1391_Fun(1); 	
	Buffer[0]	=Calendar.Hour;	
	Buffer[1]	=Calendar.Minute;
	Buffer[2]	=Calendar.Second;
	for(i=0;i<3;i++)
		AT25128_Write_Byte((Addr+i),Buffer[i]);
}	 		 	  
//============================================================================================================ 																								   
void AT25128_Write_Angle(uint16 Addr)
{	 		 
	uint8 i;
	uint8 Buffer[2];

	Buffer[0]	=Angle.time[0];	
	Buffer[1]	=Angle.time[1];	

	for(i=0;i<2;i++)
		AT25128_Write_Byte((Addr+i),Buffer[i]);
}		 		 	  
//============================================================================================================ 																								   
uint16 AT25128_Read_Angle(uint16 Addr)
{	 		 
	uint8 Buffer[2];
	TIME Temp;

	AT25128_Read_nByte(Addr,Buffer,2);
	Temp.time[0]	=Buffer[0];	
	Temp.time[1]	=Buffer[1];	 

	return(Temp.Time);
}*/ 		
