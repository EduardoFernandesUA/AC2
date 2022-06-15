#include <detpic32.h>

volatile int temperature = 15;

void confDisplays() {
    TRISB &= 0x80FF;
    TRISD &= 0xFF9F;
}

void confTimer2() {
    T2CONbits.TCKPS = 2; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR2 = 41666; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag 
}

void confADC() {
    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 2-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence
}

void refreshDisplays() {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char flag = 0;
    flag ^= 1;

    LATDbits.LATD5 = flag;
    LATDbits.LATD6 = flag^1;

    char c = flag==0 ? display7Scodes[temperature/10] : display7Scodes[temperature%10];
    LATB = (LATB & 0x80FF) | (c<<8);
}

void _int_(8) isr_T2(void) {
    refreshDisplays();
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
} 

int main(void) {
    confDisplays();
    confTimer2();
    confADC();

    EnableInterrupts();

    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 ); 
        int *p = (int *)(&ADC1BUF0);
        int sum = 0;
        int i;
        for( i = 0; i < 2; i++ ) {
            sum += p[i*4];
        }
        temperature = ((sum/2) * (65-15)) / 1023 + 15;
        printf("%d\n", temperature);
        resetCoreTimer();
        while( readCoreTimer()<2000000 );
    }
}

