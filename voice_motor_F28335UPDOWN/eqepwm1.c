#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define TB_CLK   150e6

#define PWM_CLK   5e3              // 5kHz (300rpm) EPWM1 frequency. Freq. can be changed here
#define SP        TB_CLK/PWM_CLK/2

__interrupt void epwm1_isr(void);
void initeqepwm1(void);
void InitEPwm1Example(void);
//////////////////////////////////////////////////////////////////////////////////////
//	The proper procedure for enabling ePWM clocks is as follows:
//	1. Enable ePWM module clocks in the PCLKCRx register
//	2. Set TBCLKSYNC= 0
//	3. Configure ePWM modules
//	4. Set TBCLKSYNC=1
//////////////////////////////////////////////////////////////////////////////////////
void initeqepwm1(void)
{
	InitEPwm1Gpio();						//init GPIO pins for ePWM1

	EALLOW;
	SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;	// enable PWM1
	EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    InitEPwm1Example();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

	EALLOW; 								// This is needed to write to EALLOW protected registers
	PieVectTable.EPWM1_INT = &epwm1_isr;
	EDIS;  									// This is needed to disable write to EALLOW protected registers
	IER |= M_INT3;							// Enable CPU INT3 which is connected to EPWM1-3 INT:
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;		// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
}
void InitEPwm1Example(void)
{
	EPwm1Regs.TBPRD = SP;                          // Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                      // Clear counter

	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up down
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on the scope

	EPwm1Regs.CMPA.half.CMPA = SP/2;               // Set compare A value

	EPwm1Regs.AQCTLA.all=0x60;     // CTR=CMPA when inc->EPWM1A=1, when dec->EPWM1A=0
	EPwm1Regs.AQCTLB.all=0x09;     // CTR=PRD ->EPWM1B=1, CTR=0 ->EPWM1B=0

	// Interrupt where we will modify the deadband
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                 // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;            // Generate INT on 3rd event
}

__interrupt void epwm1_isr(void)
{
   // Clear INT flag for this timer
   EPwm1Regs.ETCLR.bit.INT = 1;
   // Acknowledge this interrupt to receive more interrupts from group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}


