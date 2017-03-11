#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
#pragma CODE_SECTION(cpu_timer2_isr, "ramfuncs");

__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);

extern void freqcalc(void);
extern void speedcalc(void);
extern unsigned int sec;
extern unsigned int duty;

extern unsigned int pwm;
extern unsigned char updown_state;
extern unsigned char flag_updowm;

void initcputimer(void)
{
   InitCpuTimers();   // For this example, only initialize the Cpu Timers
   ConfigCpuTimer(&CpuTimer0, 150, 1000000/5);	// 100MHz CPU Freq  ÿ��1s��һ���ж�
   ConfigCpuTimer(&CpuTimer1, 150, 1000);		// 100MHz CPU Freq  ÿ��1ms��һ���ж�
   ConfigCpuTimer(&CpuTimer2, 150, 10000);		// 100MHz CPU Freq  ÿ��10ms��һ���ж�
   CpuTimer0Regs.TCR.all = 0x4000;
//   CpuTimer1Regs.TCR.all = 0x4000;
//   CpuTimer2Regs.TCR.all = 0x4000;
//   CpuTimer0Regs.TCR.bit.TSS = 0 ;//To start or restart the CPU-timer, set TSS bit = 0
//   CpuTimer0Regs.TCR.bit.TIE = 1 ;//To enabled the CPU-Timer interrupt, set TIE bit = 1

   EALLOW;  										// This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 = &cpu_timer0_isr;
   PieVectTable.XINT13 = &cpu_timer1_isr;
   PieVectTable.TINT2 = &cpu_timer2_isr;
   EDIS;    										// This is needed to disable write to EALLOW protected registers
   IER |= M_INT1;									// Enable CPU INT1 which is connected to CPU-Timer 0:
   IER |= M_INT13;									// Enable CPU INT13 which is connected to CPU-Timer 1:
   IER |= M_INT14;									// Enable CPU INT14 which is connected to CPU-Timer 2:
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;				// Enable TINT0 in the PIE: Group 1 __interrupt 7
}

__interrupt void cpu_timer0_isr(void)
{
	sec++;
	if(sec>59) sec=0;
//	GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 		//0x0001����12���ж�ACKnowledge�Ĵ���������ȫ������������������ж�
	if(updown_state==1)
		flag_updowm^=0x01;
//	CpuTimer0Regs.TCR.bit.TIF = 1;					// ��ʱ����ָ��ʱ�䣬��־λ��λ�������־
//	CpuTimer0Regs.TCR.bit.TRB = 1;  				// ����Timer0�Ķ�ʱ����
}

__interrupt void cpu_timer1_isr(void)
{
	pwm=9000;
	EPwm2Regs.CMPA.half.CMPA=pwm;
	duty=600;
	CpuTimer1Regs.TCR.bit.TSS = 1 ;
	CpuTimer1Regs.TCR.bit.TIE = 0 ;


//   freqcalc();
//   CpuTimer1.InterruptCount++;
//   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   // The CPU acknowledges the interrupt.
}

__interrupt void cpu_timer2_isr(void)
{
   speedcalc();
   CpuTimer2.InterruptCount++;
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   // The CPU acknowledges the interrupt.
}



