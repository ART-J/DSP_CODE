#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define SYSCLKOUT 150e6

void SetCap5Mode(void);
__interrupt void ecap5_isr(void);

extern Uint16 Freq_cap_hall;

void initcap5(void)
{
	InitECap5Gpio();
	SetCap5Mode();

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable. ECAP5_INT = &ecap5_isr;
	EDIS;
	PieCtrlRegs.PIEIER4.bit.INTx5 = 1;
	IER |= M_INT4;
}

void SetCap5Mode(void)
{
	ECap5Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
	ECap5Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
	ECap5Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

	// Configure peripheral registers
	ECap5Regs.ECCTL2.bit.CAP_APWM = 0;         // ECAPģʽ
	ECap5Regs.ECCTL2.bit.CONT_ONESHT = 0;      // multi shot
	ECap5Regs.ECCTL2.bit.STOP_WRAP = 0;        // Stop at 1 events
	ECap5Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge
//	ECap5Regs.ECCTL1.bit.CAP2POL = 1;          // Falling edge
//	ECap5Regs.ECCTL1.bit.CAP3POL = 0;          // Rising edge
//	ECap5Regs.ECCTL1.bit.CAP4POL = 1;          // Falling edge
	ECap5Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
//	ECap5Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
//	ECap5Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
//	ECap5Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
	ECap5Regs.ECCTL2.bit.SYNCI_EN = 0;         // Disable sync
	ECap5Regs.ECCTL2.bit.SYNCO_SEL = 2;        // Disable sync out
	ECap5Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable CAP1-CAP4 register loads
	ECap5Regs.ECCTL1.bit.PRESCALE = 0;
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
	ECap5Regs.ECCTL2.bit.REARM = 0;            //
	ECap5Regs.ECEINT.bit.CEVT1 = 1;            // CEVENT = interrupt
}


__interrupt void ecap5_isr(void)
{
	static unsigned int count=0;
	Uint32 temp=0;

	count++;
	temp = ECap5Regs.CAP1;
	if(count==1)
	{
		ECap5Regs.ECCTL1.bit.CAP1POL = 0;  	   // Rising edge
	}
	if(count==2)
	{
		Freq_cap_hall=SYSCLKOUT/2/temp;

		count=0;
		ECap5Regs.ECCTL1.bit.CAP1POL = 1;      // Falling edge
	}

	ECap5Regs.ECCLR.bit.CEVT1 = 1;             // CEVENT events = interrupt
	ECap5Regs.ECCLR.bit.INT = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;    // Acknowledge this interrupt to receive more interrupts from group 4
}
