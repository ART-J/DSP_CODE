// EPWM2A/B(PA2_GPIO2 & PA3_GPIO3)

#include "DSP28x_Project.h"        // Device Headerfile and Examples Include File
#define TB_CLK    150e6
#define PWM_CLK   10e3           // 10kHz EPWM1 frequency. Freq. can be changed here

#define EPWM2_TIMER_TBPRD   TB_CLK/PWM_CLK
#define EPWM2_INIT_CMPA     EPWM2_TIMER_TBPRD/2        // compare A value
unsigned int EPWM2_STEP_one  = EPWM2_TIMER_TBPRD/100;    // 占空比1%
unsigned int EPWM2_STEP_ten  = EPWM2_TIMER_TBPRD/10 ;    // 占空比10%
unsigned int EPWM2_STEP_zero_one  = EPWM2_TIMER_TBPRD/1000 ;    // 占空比0.1%

#define EPWM2_SAME_PH_MODE  0   //1表示A, B两路PWM相同，0表示A, B两路PWM相反
#define EPWM2_MIN_DB   20        //死区值=TBclk(Hz)*Tdelay(s)    3-->20nS   36

// Prototype statements for functions found within this file.
__interrupt void epwm2_isr(void);
void initepwm2(void);
void InitEPwm2Example(void);

//////////////////////////////////////////////////////////////////////////////////////
//	The proper procedure for enabling ePWM clocks is as follows:
//	1. Enable ePWM module clocks in the PCLKCRx register
//	2. Set TBCLKSYNC= 0
//	3. Configure ePWM modules
//	4. Set TBCLKSYNC=1
//////////////////////////////////////////////////////////////////////////////////////
void initepwmdb2(void)
{
	InitEPwm2Gpio();						//init GPIO pins for ePWM2

	EALLOW;
	SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;	// enable PWM2
	EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    InitEPwm2Example();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

	EALLOW; 								// This is needed to write to EALLOW protected registers
	PieVectTable.EPWM2_INT = &epwm2_isr;
	EDIS;  									// This is needed to disable write to EALLOW protected registers
	IER |= M_INT3;							// Enable CPU INT3 which is connected to EPWM1-3 INT:
	PieCtrlRegs.PIEIER3.bit.INTx2 = 1;		// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
}


__interrupt void epwm2_isr(void)
{
   // Clear INT flag for this timer
   EPwm2Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void InitEPwm2Example()							  //比较值增加，占空比减少
{
   EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;           // Set timer period
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                      // Clear counter

   // Setup TBCLK
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on the scope

   // Setup compare
   EPwm2Regs.CMPA.half.CMPA = EPWM2_INIT_CMPA;
#if   EPWM2_SAME_PH_MODE
   // Set actions
   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM2A on Zero
   EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;

   EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;             // Set PWM2A on Zero
   EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;

   // Active Low complementary PWMs - setup the deadband
   EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;
   EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
   EPwm2Regs.DBRED = EPWM2_MIN_DB;
   EPwm2Regs.DBFED = EPWM2_MIN_DB;
#else
	// Set actions
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Clear PWM2A on Period
	EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;               // Set PWM2A on event A, up count

	EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;               // Clear PWM2B on Period
	EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;             // Set PWM2B on event B, up count

	// Active Low complementary PWMs - setup the deadband
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED = EPWM2_MIN_DB;
	EPwm2Regs.DBFED = EPWM2_MIN_DB;
#endif
   // Interrupt where we will modify the deadband
   EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Zero event
   EPwm2Regs.ETSEL.bit.INTEN = 1;                 // Enable INT
   EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;            // Generate INT on 3rd event

}
