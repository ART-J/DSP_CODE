#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

extern int32 freqhz_pr,freqhz_fr;

void  initfreq1(void)
{

	EQep1Regs.QUPRD=1500000;			// Unit Timer for 100Hz at 150 MHz SYSCLKOUT

	EQep1Regs.QDECCTL.bit.QSRC=2;		// Up count mode (freq. measurement)
	EQep1Regs.QDECCTL.bit.XCR=0;        // 2x resolution (cnt falling and rising edges)

	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=00;		// QPOSCNT reset on index evnt
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timer Enable
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
	EQep1Regs.QPOSMAX=0xffffffff;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable

	EQep1Regs.QCAPCTL.bit.UPPS=3;   	// 1/8 for unit position
	EQep1Regs.QCAPCTL.bit.CCPS=7;		// 1/128 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable

}

void initeqepfreq1(void)
{
	InitEQep1Gpio();
	initfreq1();
}

void freqcalc(void)
{
     unsigned long tmp;
     unsigned long newp=0;
     static unsigned long oldp=0;

// Check unit Time out-event for speed calculation:
// Unit Timer is configured for 100Hz in INIT function
//**** Freq Calculation using QEP position counter ****//
// For a more detailed explanation of the calculation, read
// the description at the top of this file

	if(EQep1Regs.QFLG.bit.UTO==1)                  // Unit Timeout event
	{
		/** Differentiator	**/
	 	newp=EQep1Regs.QPOSLAT;                    // Latched POSCNT value

    	if (newp>oldp)
      		tmp = newp - oldp;                     // x2-x1 in v=(x2-x1)/T equation
    	else
      		tmp = (0xFFFFFFFF-oldp)+newp;

    	freqhz_fr = tmp*100/2;   // (POSCNT value)*((1/freq_fr)/2)= QUPRD* (1/SYSCLKOUT)

		// Update position counter
    	oldp = newp;
		//=======================================

		EQep1Regs.QCLR.bit.UTO=1;					// Clear __interrupt flag
	}

//**** Freq Calculation using QEP capture counter ****//
	if(EQep1Regs.QEPSTS.bit.UPEVNT==1)              // Unit Position Event
	{
		if(EQep1Regs.QEPSTS.bit.COEF==0)            // No Capture overflow
			tmp=(unsigned long)EQep1Regs.QCPRDLAT;
		else							            // Capture overflow, saturate the result
			tmp=0xFFFF;

		freqhz_pr =150000000*8/(tmp*128*2);         //QCPRDLAT value*(1/(SYSCLKOUT/CCPS))=(1/freqhz_pr)/2*UPPS

		EQep1Regs.QEPSTS.all=0x88;					// Clear Unit position event flag												     	// Clear overflow error flag
	}
}
