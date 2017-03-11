//###########################################################################
//
// FILE:    Example_2823xEpwmDeadBand.c
//
// TITLE:   Check PWM deadband generation
//
// ASSUMPTIONS:
//
//    This program requires the DSP2823x header files.
//
//    Monitor ePWM1 - ePWM3 on an Oscilloscope as described
//    below.
//
//       EPWM1A is on GPIO0
//       EPWM1B is on GPIO1
//
//       EPWM2A is on GPIO2
//       EPWM2B is on GPIO3
//
//       EPWM3A is on GPIO4
//       EPWM3B is on GPIO5
//
//    As supplied, this project is configured for "boot to SARAM"
//    operation.  The 2823x Boot Mode table is shown below.
//    For information on configuring the boot mode of an eZdsp,
//    please refer to the documentation included with the eZdsp,
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// DESCRIPTION:
//
//    This example configures ePWM1, ePWM2 and ePWM3 for:
//    - Count up/down
//    - Deadband
//
//    3 Examples are included:
//    * ePWM1: Active low PWMs
//    * ePWM2: Active low complementary PWMs
//    * ePWM3: Active high complementary PWMs
//
//    Each ePWM is configured to interrupt on the 3rd zero event
//    when this happens the deadband is modified such that
//    0 <= DB <= DB_MAX.  That is, the deadband will move up and
//    down between 0 and the maximum value.
//
//
//    View the EPWM1A/B, EPWM2A/B and EPWM3A/B waveforms
//    via an oscilloscope
//
//
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "stdio.h"
#include "string.h"

#define BUF_SIZE   16  // Sample buffer size

extern void initspieeprom(void);
extern void AT25128_Write_Byte(unsigned int Addr,unsigned char Data);
extern unsigned char AT25128_Read_Byte(unsigned int Addr);
/*--------------------------------------------------EEPROM相关函数-----*/

extern void initscic(void);
extern void scic_xmit(int a);
extern void scic_msg(char *msg);
/*-------------------------------------------------SCI_C串口相关函数----*/

extern void WDT_INIT(void);
extern void KEY_SIG_init(void);
/*-------------------------------------------------独立按键相关函数---—---*/

extern void Configio_Button(void);
extern void Key_Work(void);
/*-------------------------------------------------开发板上3x4矩阵键盘相关函数*/

extern void initgpio48(void);
extern void gpio48(Uint32 data1,Uint32 data2);
/*-------------------------------------------------48路电磁铁控制函数（需要根据硬件连法做调整，不可直接使用）*/

extern void initepwmdb2(void);//互补带死区
extern void initeqepwm1(void);//正交
/*-------------------------------------------------两种类型的PWM波-----*/

extern void initadc(void);
extern void adc_calc(void);
/*-------------------------------------------------ADC相关函数------------*/

extern void initcputimer(void);
/*-------------------------------------------------3个CUP定时器相关函数---------*/

extern void initcap5(void);
extern void initcap6(void);
/*-------------------------------------------------CAP捕获函数（中断函数内可以测频）---*/

extern void initeqepfreq1(void);
extern void initeqepspeed1(void);
/*-------------------------------------------------QEP测频相关函数-----------*/

extern void lcd12864_init(void);
extern void clrddram(void);
extern void set_coord(unsigned char x, unsigned char y);
extern void lcd12864_show_str(char* string, unsigned char num);
/*-------------------------------------------------LCD12864显示相关函数--------*/

extern char LCD_BUF_1[16];
extern char LCD_BUF_2[16];
extern char LCD_BUF_3[16];
extern char LCD_BUF_4[16];
/*---------------------------------------------------LCD缓存----------------*/

Uint16 VSense_Bus=0,ISenseU=0,ISenseW=0,ISense1=0,ISense2=0,ISense3=0,ISense4=0,ISense5=0;
int32 freqhz_pr=0,freqhz_fr=0;
int32 speed_pr=0,speed_fr=0;
Uint16 Freq_cap=0,Freq_cap_hall=0;
unsigned int dir=0;
unsigned int duty=500;
unsigned int sec=0;
unsigned int adc_finish=0;
Uint16 ReceivedChar=0;
char *msg;
unsigned char data=0;

unsigned char updown_state=1;
unsigned char flag_updowm=0;
/*----------------------------------------------------相关参数----------------*/

long uptime=1000;
unsigned int pwm=7500;
void main(void)
{
	InitSysCtrl();
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
//////////////////////////////////////////////////////////////////////////////////////////
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the F28335.cmd file.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();
//////////////////////////////////////////////////////////////////////////////////////////
//	initcap5();              //CAP5初始化
//	initcap6();              //CAP6初始化
//	initadc();               //ADC初始化
//	initgpio48();            //48路IO初始化
//	Configio_Button();       //3x4按键引脚初始化
	WDT_INIT();         	 //看门狗定时器初始化
	KEY_SIG_init();     	 //独立按键引脚初始化
	//lcd12864_init();     	 //LCD12864初始化
//	initeqepwm1();         	 //eQEPWM1初始化
	initepwmdb2();         	 //ePWMDB2初始化
	initcputimer();     	 //CPU定时器初始化
//	initeqepfreq1();	     //eQEP1freq初始化
//	initeqepspeed1();	     //eQEP1speed初始化
//	initscic();              //scic初始化
//	initspieeprom();         //eeprom初始化
//////////////////////////////////////////////////////////////////////////////////////////
	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM
//////////////////////////////////////////////////////////////////////////////////////////

//	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;	// adc开始转换
   	while(1)
   	{
/*-------------------------------------测试48io  ------------------------------------------*/
//		    gpio48(0x000000,0x000000);
//		    DELAY_US(10000);
//		    gpio48(0xffffff,0xffffff);
//		    DELAY_US(10000);
//   		GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;
/*-------------------------------------测试adc   ------------------------------------------*/
//			adc_finish=0;
//			adc_calc();
//			while(!adc_finish);
/*-------------------------------------测试cap5  ------------------------------------------*/
//			Freq_cap=Freq_cap+0;
//			Freq_cap_hall=Freq_cap_hall+0;
/*-------------------------------------测试scic  ------------------------------------------*/
//			msg = "Hello Yan Xu!\n\0";
//			scic_msg(msg);
//			while(ScicRegs.SCIRXST.bit.RXRDY !=1) { } 	//等待从上位机收到一个字节的数据
//			ReceivedChar = ScicRegs.SCIRXBUF.all;		// Get character
/*-------------------------------------测试eeprom------------------------------------------*/
//   		AT25128_Write_Byte(0x00,0x01);
//   		data=AT25128_Read_Byte(0x00);

/*-------------------------------------LCD12864spi显示-------------------------------------*/
//   		Key_Work();
   		if(updown_state==1)
   		{
   			if(flag_updowm==1)
   			{
   				EPwm2Regs.CMPA.half.CMPA=10500;	//占空比 70%
   			}
   			else
				{
   				EPwm2Regs.CMPA.half.CMPA=7000;	//占空比 70%
				}
   		}

//			if(dir==0)
//				sprintf(LCD_BUF_1,"反    M--Hz:%4d",(int)speed_fr);
//			else
//				sprintf(LCD_BUF_1,"正    M--Hz:%4d",(int)speed_fr);
//			set_coord(1, 1);
//			lcd12864_show_str(LCD_BUF_1, 16);
//
//			sprintf(LCD_BUF_2,"向    T--Hz:%4d",(int)speed_pr);
//			set_coord(1, 2);
//			lcd12864_show_str(LCD_BUF_2, 16);
//
//			sprintf(LCD_BUF_3,"%2d    U--mV:%4d",sec,VSense_Bus);
//			set_coord(1, 3);
//			lcd12864_show_str(LCD_BUF_3, 16);
//
//			sprintf(LCD_BUF_4,"%d%c   I--mV:%4d",duty,'%',ISenseU);
//			set_coord(1, 4);
//			lcd12864_show_str(LCD_BUF_4, 16);

//			sprintf(LCD_BUF_1,"计时(s):      %2d",sec);
//			set_coord(1, 1);
//			lcd12864_show_str(LCD_BUF_1,16);
//
//			sprintf(LCD_BUF_2,"uptime(ms):  %3d",(int)(uptime/1000));
//			set_coord(1, 2);
//			lcd12864_show_str(LCD_BUF_2, 16);
//
//			sprintf(LCD_BUF_3,"占空比:     %2d%c%1d",duty/10,'.',duty%10);
//			set_coord(1, 3);
//			lcd12864_show_str(LCD_BUF_3, 16);

//		    DELAY_US(100000);
   	}
}


//===========================================================================
// No more.
//===========================================================================

