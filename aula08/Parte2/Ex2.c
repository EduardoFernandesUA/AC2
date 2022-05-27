#include <detpic32.h>

volatile int count = 0;

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
    // LED0 config
    TRISEbits.TRISE0 = 1;
    // INT1 config
    TRISDbits.TRISD8 = 0;

    // Timer T2 config - 2Hz
    T2CONbits.TCKPS = 7;
    PR2 = 39063;
    TMR2 = 0;
    T2CONbits.TON = 1;
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts 
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag 

    // INT1 config
    INTCONbits.INT1EP = 0;

    EnableInterrupts();
    while(1) ;

    return 0;
}

void _int_(8) isr_T2(void) {
    if( count>0 ) {
        count -= 500;
        LATEbits.LATE0 = 1;
    } else {
        LATEbits.LATE0 = 0;
    }
    IFS0bits.T2IF = 0;
}

void _int_() isr_INT1(void) {
    count = 3000;
}
