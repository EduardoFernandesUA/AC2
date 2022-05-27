#include <detpic32.h>

volatile unsigned char voltage = 0; // Global variable

// Interrupts service routine (interrupt handler)
void _int_(27) isr_adc(void) {
    LATDBits.LATD11 = 0;
    LATD = (LATD & 0xF7FF);

    int *p = (int *)(&ADC1BUF0);
    int j, sum = 0;
    for (j = 0; j < 8; j++) {
        sum += p[j*4];
    }
    int avg = sum / 8;
    voltage = ((avg)*33+511)/1023;

    // LATDBits.LATD11 = 0;
    AD1CON1bits.ASAM = 1;
    
    LATDBits.LATD11 = 1;
    LATD = (LATD & 0xF7FF) | 0x0800;
    
    IFS1bits.AD1IF = 0; // reset AD1IF flag
}

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	static char displayFlag = 0;

	LATDbits.LATD5 = displayFlag ^ 1;
	LATDbits.LATD6 = displayFlag;
    
    LATDbits.LATD11 = displayFlag ? 1 : 0;

	char value_normalized = (value >> displayFlag*4) & 0x0F;
	char segment = display7Scodes[ value_normalized ];
	LATB = (LATB & 0x80FF) | segment << 8;

	displayFlag = displayFlag ^ 1;
}

unsigned char toBcd(unsigned char value) 
{ 
	return ((value / 10) << 4) + (value % 10); 
}

int main(void) {
    unsigned int cnt = 0;
    unsigned int baseFreq = 100;

    TRISDbits.TRISD11 = 1;

    // Display configuration
    TRISB = TRISB & 0x80FF;
	TRISD = TRISD & 0xFF9F;

    // A/D configuration
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    // interrupt system configuration
    IPC6bits.AD1IP = 2;  // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0;  // clear A/D interrupt flag
    IEC1bits.AD1IE = 1;  // enable A/D interrupts
    EnableInterrupts();
            // AD1CON1bits.ASAM = 1;

    while(1) {
        if(cnt == 0) {
        }

        send2displays(toBcd(voltage));
        cnt = (cnt + 1) % (baseFreq/5);
        delay( 1000/baseFreq );
    }

    return 0;
}
