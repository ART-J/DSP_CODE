#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

__interrupt void adc_isr(void);

// ADC start parameters
#define ADC_MODCLK 0x3   // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)     = 25.0 MHz
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/1      = 25.5MHz/(1)   = 25.0 MHz
#define ADC_SHCLK  0x1   // S/H width in ADC module periods                  = 2 ADC cycle
#define BUF_SIZE   16    // Sample buffer size

Uint16 SampleTable0[BUF_SIZE];
Uint16 SampleTable1[BUF_SIZE];
Uint16 SampleTable2[BUF_SIZE];
Uint16 SampleTable3[BUF_SIZE];
Uint16 SampleTable4[BUF_SIZE];
Uint16 SampleTable5[BUF_SIZE];
Uint16 SampleTable6[BUF_SIZE];
Uint16 SampleTable7[BUF_SIZE];

extern  Uint16  VSense_Bus,
				ISenseU,
				ISenseW,
				ISense1,
				ISense2,
				ISense3,
				ISense4,
				ISense5;

extern unsigned int adc_finish;

void initadc(void)
{
	EALLOW;
	SysCtrlRegs.HISPCP.all = ADC_MODCLK;	 // HSPCLK = SYSCLKOUT/ADC_MODCLK
	EDIS;

//	EALLOW;  								 // This is needed to write to EALLOW protected register
//	PieVectTable.ADCINT = &adc_isr;
//	EDIS;    								 // This is needed to disable write to EALLOW protected registers
//
//	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;		 // Enable ADCINT in PIE
//	IER |= M_INT1;							 // Enable CPU Interrupt 1

	InitAdc();         						 // For this example, init the ADC

	// Specific ADC setup for this example:
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;  // Sequential mode: Sample rate   = 1/[(2+ACQ_PS)*ADC clock in ns]
						//                     = 1/(3*40ns) =8.3MHz (for 150 MHz SYSCLKOUT)
						//                     = 1/(3*80ns) =4.17MHz (for 100 MHz SYSCLKOUT)
						// If Simultaneous mode enabled: Sample rate = 1/[(3+ACQ_PS)*ADC clock in ns]
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode				//级联模式
	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;        // Setup continuous run

//	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;    // Enable SEQ1 interrupt (every EOS)
//	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;  // Enable SOCA from ePWM to start SEQ1

	AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;        // Enable Sequencer override feature
	AdcRegs.ADCCHSELSEQ1.all = 0x3210;       // Initialize all ADC channel selects to A0
	AdcRegs.ADCCHSELSEQ2.all = 0x7654;
	AdcRegs.ADCCHSELSEQ3.all = 0x3210;
	AdcRegs.ADCCHSELSEQ4.all = 0x7654;
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0xf;  // convert and store in 16 results registers

//	// Assumes ePWM3 clock is already enabled in InitSysCtrl();
//	EPwm3Regs.ETSEL.bit.SOCAEN = 1;         // Enable SOC on A group
//	EPwm3Regs.ETSEL.bit.SOCASEL = 4;        // Select SOC from from CPMA on upcount
//	EPwm3Regs.ETPS.bit.SOCAPRD = 1;         // Generate pulse on 1st event
//	EPwm3Regs.CMPA.half.CMPA = 0x0080;	    // Set compare A value
//	EPwm3Regs.TBPRD = 0x927c;               // Set period for ePWM3
//	EPwm3Regs.TBCTL.bit.CTRMODE = 0;	    // count up and start
//	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;       // Clock ratio to SYSCLKOUT
//	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV2;          // Slow just to observe on the scope
}

void adc_calc(void)
{
	int i=0;
	Uint16  array_index0=0,
			array_index1=0,
			array_index2=0,
			array_index3=0,
			array_index4=0,
			array_index5=0,
			array_index6=0,
			array_index7=0,
			array_index8=0,
			array_index9=0,
			array_index10=0,
			array_index11=0,
			array_index12=0,
			array_index13=0,
			array_index14=0,
			array_index15=0;
	Uint32	value0=0,
			value1=0,
			value2=0,
			value3=0,
			value4=0,
			value5=0,
			value6=0,
			value7=0;
	for (i=0; i<BUF_SIZE; i++)
	{
		SampleTable0[i] = 0;
		SampleTable1[i] = 0;
		SampleTable2[i] = 0;
		SampleTable3[i] = 0;
		SampleTable4[i] = 0;
		SampleTable5[i] = 0;
		SampleTable6[i] = 0;
		SampleTable7[i] = 0;
	}

	for (i=0; i<BUF_SIZE; i++)
	{
		// Wait for int1
		while (AdcRegs.ADCST.bit.INT_SEQ1== 0);
		AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;

		SampleTable0[array_index0++]= ( (AdcRegs.ADCRESULT0)>>4);
		SampleTable1[array_index1++]= ( (AdcRegs.ADCRESULT1)>>4);
		SampleTable2[array_index2++]= ( (AdcRegs.ADCRESULT2)>>4);
		SampleTable3[array_index3++]= ( (AdcRegs.ADCRESULT3)>>4);
		SampleTable4[array_index4++]= ( (AdcRegs.ADCRESULT4)>>4);
		SampleTable5[array_index5++]= ( (AdcRegs.ADCRESULT5)>>4);
		SampleTable6[array_index6++]= ( (AdcRegs.ADCRESULT6)>>4);
		SampleTable7[array_index7++]= ( (AdcRegs.ADCRESULT7)>>4);

		SampleTable0[array_index8++] = ( (AdcRegs.ADCRESULT8)>>4);
		SampleTable1[array_index9++] = ( (AdcRegs.ADCRESULT9)>>4);
		SampleTable2[array_index10++]= ( (AdcRegs.ADCRESULT10)>>4);
		SampleTable3[array_index11++]= ( (AdcRegs.ADCRESULT11)>>4);
		SampleTable4[array_index12++]= ( (AdcRegs.ADCRESULT12)>>4);
		SampleTable5[array_index13++]= ( (AdcRegs.ADCRESULT13)>>4);
		SampleTable6[array_index14++]= ( (AdcRegs.ADCRESULT14)>>4);
		SampleTable7[array_index15++]= ( (AdcRegs.ADCRESULT15)>>4);
	}

	for (i=0; i<BUF_SIZE; i++)
	{
		value0 += SampleTable0[i];
		value1 += SampleTable1[i];
		value2 += SampleTable2[i];
		value3 += SampleTable3[i];
		value4 += SampleTable4[i];
		value5 += SampleTable5[i];
		value6 += SampleTable6[i];
		value7 += SampleTable7[i];
	}
	value0 = (value0 / BUF_SIZE-20) *1000 *3/4096 ;
	value1 = (value1 / BUF_SIZE-20) *1000 *3/4096 ;
	value2 = (value2 / BUF_SIZE-20) *1000 *3/4096 ;
	value3 = (value3 / BUF_SIZE-20) *1000 *3/4096 ;
	value4 = (value4 / BUF_SIZE-20) *1000 *3/4096 ;
	value5 = (value5 / BUF_SIZE-20) *1000 *3/4096 ;
	value6 = (value6 / BUF_SIZE-20) *1000 *3/4096 ;
	value7 = (value7 / BUF_SIZE-20) *1000 *3/4096 ;

	VSense_Bus	= value0;
	ISenseU		= value1;
	ISenseW		= value2;
	ISense1		= value3;
	ISense2		= value4;
	ISense3		= value5;
	ISense4		= value6;
	ISense5		= value7;

	adc_finish=1;
}



