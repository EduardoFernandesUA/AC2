#include <detpic32.h>

volatile int count = 0;

int main(void) {
    T3CONbits.TCKPS = 7;
    PR3 = 39063;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T3IE = 1; // Enable timer T2 interrupts 
    IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag 


    EnableInterrupts();
    while(1) ;

    return 0;
}

void _int_(12) isr_T3(void) {
    count = (count+1) % 2;
    if( count==0 ) {
        putChar('.');
    }
    IFS0bits.T3IF = 0;
}
