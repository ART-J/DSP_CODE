#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define CAP_MODE 0              //计算固定频率方波;       0:多周期捕获计算均值;      1:单周期捕获

#define SYSCLKOUT 150e6
#define CAP_COUNT 32

void SetCap6Mode(void);
__interrupt void ecap6_isr(void);

Uint32 cap_value_all=0;
Uint16 cap_value[CAP_COUNT];
extern Uint16 Freq_cap;

void initcap6(void)
{
	InitECap6Gpio();
	SetCap6Mode();

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable. ECAP6_INT = &ecap6_isr;
	EDIS;
	PieCtrlRegs.PIEIER4.bit.INTx6 = 1;
	IER |= M_INT4;
}

void SetCap6Mode(void)
{
	ECap6Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
	ECap6Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
	ECap6Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

	// Configure peripheral registers
	ECap6Regs.ECCTL2.bit.CAP_APWM = 0;         // ECAP模式
	ECap6Regs.ECCTL2.bit.CONT_ONESHT = 0;      // multi shot
	ECap6Regs.ECCTL2.bit.STOP_WRAP = 0;        // Stop at 1 events
	ECap6Regs.ECCTL1.bit.CAP1POL = 0;          // 默认配置上升沿触发
//	ECap6Regs.ECCTL1.bit.CAP2POL = 1;          // Falling edge
//	ECap6Regs.ECCTL1.bit.CAP3POL = 0;          // Rising edge
//	ECap6Regs.ECCTL1.bit.CAP4POL = 1;          // Falling edge
	ECap6Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
//	ECap6Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
//	ECap6Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
//	ECap6Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
	ECap6Regs.ECCTL2.bit.SYNCI_EN = 0;         // Disable sync
	ECap6Regs.ECCTL2.bit.SYNCO_SEL = 2;        // Disable sync out
	ECap6Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable CAP1-CAP4 register loads
	ECap6Regs.ECCTL1.bit.PRESCALE = 0;
	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
	ECap6Regs.ECCTL2.bit.REARM = 0;            //
	ECap6Regs.ECEINT.bit.CEVT1 = 1;            // CEVENT = interrupt
}


__interrupt void ecap6_isr(void)
{
	unsigned int i;
	static unsigned int count=0;
	Uint32 temp=0;
	count++;
	temp = ECap6Regs.CAP1;

#if CAP_MODE
	if(count==2)
	{
		Freq_cap=SYSCLKOUT/temp;
		count=0;
	}
#else
	if(count>1)
	{
		cap_value[count-2] = SYSCLKOUT/temp;
	}
	if(count==CAP_COUNT+1)
	{
		count=0;
		for(i=0;i<CAP_COUNT;i++)
		{
			cap_value_all += cap_value[i];
		}
		Freq_cap=cap_value_all/CAP_COUNT;
		cap_value_all=0;
	}
#endif
	ECap6Regs.ECCLR.bit.CEVT1 = 1;            // CEVENT events = interrupt
	ECap6Regs.ECCLR.bit.INT = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;  // Acknowledge this interrupt to receive more interrupts from group 4
}
