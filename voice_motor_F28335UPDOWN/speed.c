#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

extern int32 speed_pr,speed_fr;
extern unsigned int dir;

void initspeed1(void)
{

    EQep1Regs.QUPRD=1500000;            // Unit Timer for 100Hz at 150 MHz SYSCLKOUT

    EQep1Regs.QDECCTL.bit.QSRC=0x00;      // QEP quadrature count mode

    EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep1Regs.QEPCTL.bit.PCRM=00;       // PCRM=00 mode - QPOSCNT reset on index event
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out
    EQep1Regs.QPOSMAX=0xffffffff;
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    EQep1Regs.QCAPCTL.bit.UPPS=3;       // 1/8 for unit position
    EQep1Regs.QCAPCTL.bit.CCPS=7;       // 1/128 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable


}

void initeqepspeed1(void)
{
	InitEQep1Gpio();
	initspeed1();
}

void speedcalc(void)
{
	 unsigned long Tmp1=0,Tmp2=0,newp=0;
     static unsigned long oldp=0;
//**** Position calculation - mechanical and electrical motor angle  ****//
     dir = EQep1Regs.QEPSTS.bit.QDF;    // Motor direction: 0=CCW/reverse, 1=CW/forward

// Check an index occurrence
     if (EQep1Regs.QFLG.bit.IEL == 1)
     {
        EQep1Regs.QCLR.bit.IEL=1;                   // Clear __interrupt flag
     }


//**** High Speed Calculation using QEP Position counter ****//
// Check unit Time out-event for speed calculation:
// Unit Timer is configured for 100Hz in INIT function

    if(EQep1Regs.QFLG.bit.UTO==1)                    // If unit timeout (one 100Hz period)
    {
		/** Differentiator	**/
	 	newp=EQep1Regs.QPOSLAT;                    // Latched POSCNT value

        if (dir==0)                     			// POSCNT is counting down
        {
            if (newp>oldp)
                Tmp1 = 0xFFFFFFFF - newp + oldp;    				// x2-x1 should be negative
            else
                Tmp1 = oldp - newp;
        }
        else if (dir==1)                			// POSCNT is counting up
        {
            if (newp<oldp)
            	Tmp1 = 0xFFFFFFFF - oldp + newp;                 // x2-x1 should be positive
            else
            	Tmp1 = newp - oldp;
        }

    	speed_fr = Tmp1*100/4;   					// (POSCNT value)*((1/freq_fr)/4)= QUPRD* (1/SYSCLKOUT)

		// Update position counter
    	oldp = newp;
		//=======================================

        EQep1Regs.QCLR.bit.UTO=1;                   // Clear __interrupt flag
    }

//**** Freq Calculation using QEP capture counter ****//
	if(EQep1Regs.QEPSTS.bit.UPEVNT==1)              // Unit Position Event
	{
		if(EQep1Regs.QEPSTS.bit.COEF==0)            // No Capture overflow
			Tmp2=(unsigned long)EQep1Regs.QCPRDLAT;
		else							            // Capture overflow, saturate the result
			Tmp2=0xFFFF;

		speed_pr =150000000*8/(Tmp2*128*4);      //QCPRDLAT value*(1/(SYSCLKOUT/CCPS))=(1/freqhz_pr)/4*UPPS

		EQep1Regs.QEPSTS.all=0x88;					    // Clear Unit position event flag												     	// Clear overflow error flag
	}
}

