#include "DSP28x_Project.h"

#define  K0   0
#define  K1   1
#define  K2   2
#define  K3   3
#define  K4   4
#define  K5   5
#define  K6   6
#define  K7   7
#define  K8   8
#define  K9   9
//#define  UP   10
//#define  DW   11
//#define  LF   12
//#define  RT   13
#define  AD   14   //+
#define  SB   15   //-

#define  L_O_V_I()     EALLOW;                             \
                       GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1; \
					   GpioDataRegs.GPBDAT.bit.GPIO48 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO24 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO24 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO25 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO25 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO26 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO26 = 0; DELAY_US(1);\
					   \
					   \
					   GpioCtrlRegs.GPADIR.bit.GPIO21 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO21 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO27 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO27 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO20 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO20 = 0; DELAY_US(1);\
					   EDIS;
#define  L_I_V_O()     EALLOW;                             \
                       GpioCtrlRegs.GPBDIR.bit.GPIO48 = 0; \
					   GpioDataRegs.GPBDAT.bit.GPIO48 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO24 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO24 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO25 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO25 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO26 = 0; \
					   GpioDataRegs.GPADAT.bit.GPIO26 = 0; DELAY_US(1);\
					   \
					   \
					   GpioCtrlRegs.GPADIR.bit.GPIO21 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO21 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO27 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO27 = 0; DELAY_US(1);\
					   GpioCtrlRegs.GPADIR.bit.GPIO20 = 1; \
					   GpioDataRegs.GPADAT.bit.GPIO20 = 0; DELAY_US(1);\
					   EDIS;   

#define	  V2 	GpioDataRegs.GPADAT.bit.GPIO21
//#define	  V1 	GpioDataRegs.GPBDAT.bit.GPIO49
#define	  V4 	GpioDataRegs.GPADAT.bit.GPIO27
#define	  V3 	GpioDataRegs.GPADAT.bit.GPIO20

#define	  L1 	GpioDataRegs.GPADAT.bit.GPIO26
#define	  L2 	GpioDataRegs.GPADAT.bit.GPIO25
#define	  L3 	GpioDataRegs.GPBDAT.bit.GPIO48
#define	  L4 	GpioDataRegs.GPADAT.bit.GPIO24 


void delay(Uint16 t);
void Configio_Button(void);
void Key_Work(void);

void delay(Uint16 t)
{
   Uint16 i;
   while(t--)
    {
      for(i=0;i<125;i++);
    }
}

Uint16 Scan_Button(void){
   Uint16 i=0, num=0xFFFF, x=0, y=0;
   i = V2&V3&V4;  //如果列读取到有低电平说明有按键按下
   if(i == 0){
     DELAY_US(30000);//延时消抖
     i = V2&V3&V4;  
	 if(i == 0){	//如果列读取到仍然有低电平说明有按键按下
       // if(!V1) x= 0x10;
		if(!V2) x= 0x20;
		if(!V3) x= 0x30;
		if(!V4) x= 0x40;
		L_I_V_O();DELAY_US(200); //行与列输入输出反转		
		if(!L2) y= 0x02;
		if(!L3) y= 0x03;
		if(!L4) y= 0x04;
		if(!L1) y= 0x01;
		x |= y;	
        L_O_V_I();	
		switch(x){
          case 0x21: num = K1;break;
		  case 0x22: num = K4;break;
		  case 0x23: num = SB;break;
		  case 0x24: num = K7;break;
		  case 0x31: num = K2;break;
		  case 0x32: num = K5;break;
		  case 0x33: num = K0;break;
		  case 0x34: num = K8;break;
		  case 0x41: num = K3;break;
		  case 0x42: num = K6;break;
		  case 0x43: num = AD;break;
		  case 0x44: num = K9;break;
		        
          default: num=0xFFFF; 
		}
		
	 }
   }
   return num;
}

void Configio_Button(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0; 
    EDIS;
	L_O_V_I();  //行设置为输出（输出低电平）列设置为输入
}

void Key_Work(void)
{
	  Uint16 key_value;
      key_value = Scan_Button();
      if( key_value != 0xFFFF )
      {
        switch(key_value)
        {
		  case AD:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case SB:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K0:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K1:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K2:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K3:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K4:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K5:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K6:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K7:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K8:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
		  case K9:    GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; break;
          default:  break;
		}
	  }
}

