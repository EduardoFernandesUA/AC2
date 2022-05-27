#include <detpic32.h>

// GLOBAL VARIABLES
volatile int voltage = 0;

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	static char displayFlag = 0;

	LATDbits.LATD5 = displayFlag ^ 1;
	LATDbits.LATD6 = displayFlag;

	char value_normalized = (value >> displayFlag*4) & 0x0F;
	char segment = display7Scodes[ value_normalized ];
	LATB = (LATB & 0x80FF) | segment << 8;

	displayFlag = displayFlag ^ 1;
}

unsigned char toBcd(unsigned char value) 
{ 
	return ((value / 10) << 4) + (value % 10); 
}

void configureAll() {
    // Timer T1 - 5Hz
    T1CONbits.TCKPS = 6;
    PR1 = 3125;
    TMR1 = 0;
    T1CONbits.TON = 1;
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T1IE = 1; // Enable timer T3 interrupts 

    // Timer T2 - 100Hz
    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;
    T3CONbits.TON = 1;
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts 

    // Display and Leds
    TRISB = (TRISB & 0x80F0) | 0x000F;
	TRISD = (TRISD & 0xFF9F);
    
    // A/D module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8-1; // 8 samples
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    IPC6bits.AD1IP = 2;  // configure priority of A/D interrupts
    IEC1bits.AD1IE = 1;  // enable A/D interrupts
}

void _int_(4) isr_T1(void) { // read ADC
    IFS1bits.AD1IF = 0; // Reset A/D interrupt flag
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void) { // send info to displays
    send2displays(toBcd(voltage));
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}

void _int_(27) isr_adc(void) {
    int *p = (int *)(&ADC1BUF0);
    int j, sum = 0;
    for (j = 0; j < 8; j++) {
        sum += p[j*4];
    }
    int avg = sum / 8;
    voltage = ((avg)*33+511)/1023;
}

int main(void) {
    configureAll();

    // Reset AD1IF, T1IF and T3IF flags
    IFS1bits.AD1IF = 0;
    IFS0bits.T1IF = 0; // Reset timer T3 interrupt flag 
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag 

    EnableInterrupts(); 
    while(1);
    return 0;
}
