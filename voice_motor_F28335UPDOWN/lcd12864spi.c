////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   ʹ��lcd12864.c֮ǰ�Ȱ����º��������ŵ�main.c������
// ����lcd12864.c������Ҫ�õ�������
//extern void lcd12864_init(void);
//extern void clrddram(void);
//extern void set_coord(unsigned char x, unsigned char y);
//extern void lcd12864_show_str(unsigned char* string, unsigned char num);

//extern unsigned char LCD_BUF_1[16];
//extern unsigned char LCD_BUF_2[16];
//extern unsigned char LCD_BUF_3[16];
//extern unsigned char LCD_BUF_4[16];
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define SCLK_H    GpioDataRegs.GPBSET.bit.GPIO57 = 1;  	 //ʱ�����ø�
#define SCLK_L    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;  //ʱ�����õ�
                                    //�����������Ŷ�����SID�����У����б�Ҫ��ת��SID()�����޸�

//************ָ��궨��*************************************
// ����ָ��
#define    ext8      0x34	// 8λ���ݣ�����ָ�����
#define    clr       0x01	// DDRAM����"20H",��λ��AC
#define    cursor_1  0x06	// �����ƶ�����->��
#define    texton    0x0c	// ������ʾON,  �α�OFF, �α�λ�÷�������OFF
#define    textoff   0x08	// ������ʾOFF, �α�OFF, �α�λ�÷�������OFF
#define    ddbas     0x80	// �趨DDRAM ��ַ,��һ��80H~87H,�ڶ���90H~97H
#define    cgbas     0x40	// CGRAM����ַ����Χ��cgbas��cabas+64��ÿһλ��ַ����2�ֽ�

// ����ָ��
#define    bas8      0x30	// 8λ���ݣ�����ָ�����
#define    grapon    0x36	// ����ָ�ͼ����ʾ��
//************************************************************

char LCD_BUF_1[16] ={""};
char LCD_BUF_2[16] ={""};
char LCD_BUF_3[16] ={""};
char LCD_BUF_4[16] ={""};

//********************************************************************************
//* ��    �ƣ�lcdio()
//* ��    �ܣ�ʱ�����ţ�������������
//* ��ڲ���
//* ��Ҫ˵������SID()����ͬ���޸�
//********************************************************************************
void lcdio(void)
{
   EALLOW;
   GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pullup on GPIO57
   GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;  // GPIO57 = GPIO57
   GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;   // GPIO57 = output

   GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pullup on GPIO56
   GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;  // GPIO56 = GPIO56
   GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;   // GPIO56 = output
   EDIS;
}

//********************************************************************************
//* ��    �ƣ�SID()
//* ��    �ܣ������������
//* ��ڲ�����SID(0x00)---�õ�,         SID(0x01)---�ø�
//* ��Ҫ˵�����˺����� ---GPIO56---������Ϊ�����������������䶯�����޸�GPIO���żĴ���
//********************************************************************************
void SID(unsigned char data)
{
	EALLOW;
	GpioDataRegs.GPBDAT.bit.GPIO56 = ((data&0x80)>>7);
	//GpioDataRegs.GPxDAT.bit.GPIOxx = ((data&0x80)>>7);//ʾ��
	EDIS;
}

void delaynms(unsigned int di)                       //������ʱ ��������Ҫ�޸�
{ 
	unsigned int da,db;
	for(da=0;da<di;da++)
	for(db=0;db<10;db++);
}

void sendbyte(unsigned char bbyte)      //����һ���ֽ�
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SID(bbyte);                    //ȡ�����λ
		SCLK_H;
		SCLK_L;
		bbyte<<=1;                     //����
	}
}

//********************************************************************************
//* ��    �ƣ�write()
//* ��    �ܣ���Һ��ģ��дָ�������
//* ��ڲ�����start---0:дָ��,1:д����                  ddata---ָ�������
//* ���ز�����
//********************************************************************************
void write(unsigned char start, unsigned char ddata) //дָ�������
{ 
	unsigned char start_data,Hdata,Ldata;

	if(start==0)  start_data=0xf8;	 //дָ��
	  else        start_data=0xfa;   //д����

	Hdata=ddata&0xf0;	  			 //ȡ����λ
	Ldata=(ddata<<4)&0xf0;			 //ȡ����λ
	sendbyte(start_data);	 		 //������ʼ�ź�
	delaynms(20); 					 //��ʱ�Ǳ����
	sendbyte(Hdata);				 //���͸���λ
	delaynms(20); 					 //��ʱ�Ǳ����
	sendbyte(Ldata);				 //���͵���λ
	delaynms(20); 					 //��ʱ�Ǳ����
}

void lcd12864_init(void)	   		 //��ʼ��LCD
{ 
	lcdio();

	write(0,bas8);      		     //8 λ���ݣ�����ָ�
	write(0,texton);    		     //��ʾ�򿪣����أ����׹�
	write(0,clr);       		     //��������DDRAM�ĵ�ַ����������
}

//********************************************************************************
//* ��    �ƣ�clrddram()
//* ��    �ܣ�����
//* ��ڲ�����
//* ���ز�����void
//********************************************************************************
void clrddram(void)
{
    write(0,clr);
    write(0,ext8);
    write(0,bas8);
}

//********************************************************************************
//* ��    �ƣ�set_coord()
//* ��    �ܣ�����DDRAM����ʾ����
//* ��ڲ�����xȡֵ��Χ��1��8,  yȡֵ��Χ1��4
//* ���ز�����void
//********************************************************************************
void set_coord(unsigned char x, unsigned char y)
{
    switch(y)
    {
        case 1: write(0,0x7F + x);break;
        case 2: write(0,0x8F + x);break;
        case 3: write(0,0x87 + x);break;
        case 4: write(0,0x97 + x);break;
        default:                  break;
    }
}

//********************************************************************************
//* ��    �ƣ�lcd12864_show_str()
//* ��    �ܣ�����Ļ����ʾһ���ַ���
//* ��ڲ�����string---�ַ������׵�ַ,num---��ʾ�ַ����ĸ���
//* ���ز�����void
//********************************************************************************
void lcd12864_show_str(char* string, unsigned char num)
{
    while(num--)
    {
        write(1,*string++);//��12864Һ��ģ�鴫������
    }
}

//void lcd_setxy(unsigned char X, unsigned char Y)
//{
//   switch(Y) {
//		case 1: write(0,0x7F + X);break;
//		case 2: write(0,0x8F + X);break;
//		case 3: write(0,0x87 + X);break;
//		case 4: write(0,0x97 + X);break;
//                default:break;
//	    }
//}

//void lcd12864_disp_str(unsigned char X, unsigned char Y, unsigned char*Str)
//{
//    unsigned char temp;
//    lcd_setxy(X, Y);
//    temp = *Str;
//    while(temp != 0)
//    {
//        write(1,temp);
//        temp = *(++Str);
//    }
//}
