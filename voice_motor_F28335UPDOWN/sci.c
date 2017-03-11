#include "DSP28x_Project.h"

// Prototype statements for functions found within this file.
void scic_echoback_init(void);
void scic_xmit(int a);
void scic_msg(char *msg);

void initscic(void)
{
	 InitScicGpio();
	 scic_echoback_init();
}

// Test 1,SCIC  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scic_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0003;
	ScicRegs.SCICTL2.bit.TXINTENA = 1;
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;

	ScicRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz(default)
	ScicRegs.SCILBAUD    =0x00E7;

	ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

// Transmit a character from the SCI
void scic_xmit(int a)
{
    while (ScicRegs.SCICTL2.bit.TXRDY == 0) {}
    ScicRegs.SCITXBUF=a;
}

void scic_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scic_xmit(msg[i]);
        i++;
    }
}


