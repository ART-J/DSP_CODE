#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

void initgpio48(void)
{
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;  	 // Enable pullup
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0; 	 // GPIO = GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;  	 // GPIO = output

	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;  	 // Enable pullup
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;  	 // GPIO = GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;      // GPIO = output

	GpioCtrlRegs.GPBPUD.all |= 0x00000000;   // Enable pullup
	GpioCtrlRegs.GPBMUX1.all |= 0x0000;      // GPIO = GPIO
	GpioCtrlRegs.GPBMUX2.all |= 0x0000;      // GPIO = GPIO
	GpioCtrlRegs.GPBDIR.all |= 0x3ffcffc0;   // GPIO = output

	GpioCtrlRegs.GPCPUD.all |= 0x00000000;    // Enable pullup
	GpioCtrlRegs.GPCMUX1.all |= 0x0000;       // GPIO = GPIO
	GpioCtrlRegs.GPCMUX2.all |= 0x0000;       // GPIO = GPIO
	GpioCtrlRegs.GPCDIR.all |= 0x0fffffff;    // GPIO = output
	EDIS;
}

void gpio48(Uint32 data1,Uint32 data2)
{
	EALLOW;
	GpioDataRegs.GPADAT.bit.GPIO28 = data1&0x01;        //��A1λ��

	GpioDataRegs.GPADAT.bit.GPIO31 = (data1&0x02)>>1;   //��A2λ��

	GpioDataRegs.GPBDAT.all = (GpioDataRegs.GPBDAT.all&0xc003003f) + ((data1&0xffc)<<4) + ((data1&0xfff000)<<6)   ;//��A3~A24λ��

	GpioDataRegs.GPCDAT.all = data2;                    //��B1~B24λ��
	EDIS;
}

