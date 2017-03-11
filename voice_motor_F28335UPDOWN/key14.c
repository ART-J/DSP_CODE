////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   ʹ��keybord.c֮ǰ�Ȱ����º��������ŵ�main.c������
// ����keybord.c������Ҫ�õ�������
//extern __interrupt void wakeint_isr(void);
//extern void WDT_INIT(void);
//extern void KEY_SIG_init(void);
//extern void KEY_IODect(void);
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define KEY_H_VALUE GpioDataRegs.GPADAT.bit.GPIO11
#define KEY_G_VALUE GpioDataRegs.GPADAT.bit.GPIO10
#define KEY_F_VALUE GpioDataRegs.GPADAT.bit.GPIO9
#define KEY_E_VALUE GpioDataRegs.GPADAT.bit.GPIO8

#define KEY_A_VALUE GpioDataRegs.GPADAT.bit.GPIO7
#define KEY_B_VALUE GpioDataRegs.GPADAT.bit.GPIO6
#define KEY_C_VALUE GpioDataRegs.GPADAT.bit.GPIO5
#define KEY_D_VALUE GpioDataRegs.GPADAT.bit.GPIO4

extern unsigned int EPWM2_STEP_zero_one; // ռ�ձ�0.1%
extern unsigned int EPWM2_STEP_one;    // ռ�ձ�1%
extern unsigned int EPWM2_STEP_ten;    // ռ�ձ�10%
extern unsigned int duty;

extern long uptime;
extern unsigned int pwm;

extern unsigned char updown_state;
extern unsigned char flag_updowm;
/******************************************************************************************************
 * ��       �ƣ�Px.x_Onclick()
 * ��       �ܣ�Px.x���ж��¼�������������Px.x�������º���һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�����������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void KEY_A_Onclick()					//KEY_A���¼�������
{
//	EPwm1Regs.CMPA.half.CMPA-=EPWM2_STEP_ten;
//	EPwm2Regs.CMPA.half.CMPA-=EPWM2_STEP_ten;
////	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//	duty+=10;

	static int temp=0;
	temp++;
	if((temp%3)==1)
	{
		uptime=10000;
		ConfigCpuTimer(&CpuTimer1, 100, 10000);
	}
	if((temp%3)==2)
	{
		uptime=100000;
		ConfigCpuTimer(&CpuTimer1, 100, 100000);
	}
	if((temp%3)==0)
	{
		uptime=1000;
		ConfigCpuTimer(&CpuTimer1, 100, 1000);
	}
	EPwm2Regs.CMPA.half.CMPA=pwm;
}

void KEY_B_Onclick()					//KEY_B���¼�������
{
//	EPwm1Regs.CMPA.half.CMPA+=EPWM2_STEP_ten;
//	EPwm2Regs.CMPA.half.CMPA+=EPWM2_STEP_ten;
////	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//	duty-=10;


	CpuTimer1Regs.TCR.all = 0x4001;
	EPwm2Regs.CMPA.half.CMPA=pwm;
	duty=pwm/15;
}

void KEY_C_Onclick()					//KEY_C���¼�������
{
//	EPwm1Regs.CMPA.half.CMPA-=EPWM2_STEP_one;
//	EPwm2Regs.CMPA.half.CMPA-=EPWM2_STEP_one;
////	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//	duty+=1;

	pwm-=EPWM2_STEP_zero_one;
	duty=pwm/15;
}

void KEY_D_Onclick()					//KEY_D���¼�������
{
//	EPwm1Regs.CMPA.half.CMPA+=EPWM2_STEP_one;
//	EPwm2Regs.CMPA.half.CMPA+=EPWM2_STEP_one;
////	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//	duty-=1;

	pwm+=EPWM2_STEP_zero_one;
	duty=pwm/15;
}

void KEY_E_Onclick()					//KEY_E���¼�������
{
	pwm-=EPWM2_STEP_one;
	duty=pwm/15;
}

void KEY_F_Onclick()					//KEY_F���¼�������
{
	pwm+=EPWM2_STEP_one;
	duty=pwm/15;
}

void KEY_G_Onclick()					//KEY_G���¼�������
{
	pwm=9000;
	EPwm2Regs.CMPA.half.CMPA=pwm;
	duty=600;
}

void KEY_H_Onclick()					//KEY_H���¼�������
{
	updown_state^=1;
	if(updown_state==1)
		ConfigCpuTimer(&CpuTimer0, 150, 1000000/10);	// 100MHz CPU Freq  ÿ��1s��һ���ж�
	else
	{
		ConfigCpuTimer(&CpuTimer0, 150, 1000000);	// 100MHz CPU Freq  ÿ��1s��һ���ж�
		EPwm2Regs.CMPA.half.CMPA=9000;
		flag_updowm=0;
	}
}

/******************************************************************************************************
 * ��       �ƣ�KEY_SIG_init()
 * ��       �ܣ���ʼ��GPIO��Ϊ����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ���������������裬�˿ڸ���GPIO���ܣ���������
 * ��       ������
 ******************************************************************************************************/
void KEY_SIG_init(void)
{
	   // Make GPIO as input
   EALLOW;
   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;



   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO9 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO10 = 0;

   GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;
   EDIS;
}

/******************************************************************************************************
 * ��       �ƣ�KEY_IODect()
 * ��       �ܣ��ж��Ƿ��м������£��ĸ��������£���������ӦIO���ж��¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����������������ɨ��Ľ������֪�������Ƿ񱻰���
 * ��       ������
 ******************************************************************************************************/
void KEY_IODect(void)
{
	static unsigned char  KEY_A_Now=0, KEY_B_Now=0, KEY_C_Now=0, KEY_D_Now=0,
						  KEY_E_Now=0,KEY_F_Now=0, KEY_G_Now=0, KEY_H_Now=0;	//����ֵ������ʱ�豣��
	unsigned char         KEY_A_Past=0,KEY_B_Past=0,KEY_C_Past=0,KEY_D_Past=0,
						  KEY_E_Past=0,KEY_F_Past=0,KEY_G_Past=0,KEY_H_Past=0;

	KEY_A_Past=KEY_A_Now;
	KEY_B_Past=KEY_B_Now;
	KEY_C_Past=KEY_C_Now;
	KEY_D_Past=KEY_D_Now;

	KEY_E_Past=KEY_E_Now;
	KEY_F_Past=KEY_F_Now;
	KEY_G_Past=KEY_G_Now;
	KEY_H_Past=KEY_H_Now;

	//-----��ѯIO������Ĵ���-----
	if(0!=KEY_A_VALUE)	KEY_A_Now=1;
		else 			KEY_A_Now=0;

	if(0!=KEY_B_VALUE)	KEY_B_Now=1;
		else 		    KEY_B_Now=0;

	if(0!=KEY_C_VALUE)	KEY_C_Now=1;
		else 		    KEY_C_Now=0;

	if(0!=KEY_D_VALUE)	KEY_D_Now=1;
		else 		    KEY_D_Now=0;


	if(0!=KEY_E_VALUE)	KEY_E_Now=1;
		else 			KEY_E_Now=0;

	if(0!=KEY_F_VALUE)	KEY_F_Now=1;
		else 		    KEY_F_Now=0;

	if(0!=KEY_G_VALUE)	KEY_G_Now=1;
		else 		    KEY_G_Now=0;

	if(0!=KEY_H_VALUE)	KEY_H_Now=1;
		else 		    KEY_H_Now=0;


	//-----ǰһ�θߵ�ƽ����һ�ε͵�ƽ��˵����������-----
	if((KEY_A_Past==1)&&(KEY_A_Now==0)) KEY_A_Onclick();
	if((KEY_B_Past==1)&&(KEY_B_Now==0)) KEY_B_Onclick();
	if((KEY_C_Past==1)&&(KEY_C_Now==0)) KEY_C_Onclick();
	if((KEY_D_Past==1)&&(KEY_D_Now==0)) KEY_D_Onclick();

	if((KEY_E_Past==1)&&(KEY_E_Now==0)) KEY_E_Onclick();
	if((KEY_F_Past==1)&&(KEY_F_Now==0)) KEY_F_Onclick();
	if((KEY_G_Past==1)&&(KEY_G_Now==0)) KEY_G_Onclick();
	if((KEY_H_Past==1)&&(KEY_H_Now==0)) KEY_H_Onclick();
}

/******************************************************************************************************
 * ��       �ƣ�wakeint_isr()
 * ��       �ܣ���ӦWDT��ʱ�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����WDT��ʱ�ж϶�ռ�ж����������������һ���ж��ж��¼���Ҳ�����˹������־λ��
 *                  ���ԣ���WDT��ʱ�жϷ����Ӻ����У�ֱ�ӵ���WDT�¼��������Ϳ����ˡ�
 * ��       ������
 ******************************************************************************************************/
__interrupt void wakeint_isr(void)
{
	KEY_IODect();		//���ͨ�����������¼�������
	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/******************************************************************************************************
 * ��       �ƣ�WDT_INIT()
 * ��       �ܣ��趨WDT��ʱ�ж�Ϊ16ms������WDT��ʱ�ж�ʹ��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����WDT��ʱ�жϵ�ʱ��Դѡ��ACLK��
 * ��       ������
 ******************************************************************************************************/
void WDT_INIT(void)
{
	EALLOW;                              // This is needed to write to EALLOW protected registers
	PieVectTable.WAKEINT = &wakeint_isr; // Connect the watchdog to the WAKEINT interrupt of the PIE
	SysCtrlRegs.SCSR = BIT1;             // Write to the whole SCSR register to avoid clearing WDOVERRIDE bit
	EDIS;                                // Enable WAKEINT in the PIE: Group 1 interrupt 8
                                         // Enable INT1 which is connected to WAKEINT:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx8 = 1;   // Enable PIE Group 1 INT8
	IER |= M_INT1;                       // Enable CPU INT1
	//EINT;                                // Enable Global Interrupts

	ServiceDog();                        // Reset the watchdog counter
	EALLOW;
	SysCtrlRegs.WDCR = 0x0028;           // Enable the watchdog
	EDIS;                                // This is needed to disable write to EALLOW protected registers
}
