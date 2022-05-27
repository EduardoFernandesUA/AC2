#include <detpic32.h>

int main(void) {
    // RD0 & RD2 config
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD2 = 1;

    // Timer 1 config
    T1CONbits.TCKPS = 6;
    PR1 = 62499;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T1IE = 1; // Enable timer T3 interrupts 
    IFS0bits.T1IF = 0; // Reset timer T3 interrupt flag 

    // Timer 3 config
    T3CONbits.TCKPS = 6;
    PR3 = 12500;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6]) 
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts 
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag 

    EnableInterrupts();

    while(1);
    return 1;
}

void _int_(4) isr_T1(void) {
    LATDbits.LATD0 = !LATDbits.LATD0;
    IFS0bits.T1IF = 0; // Reset timer T3 interrupt flag
}

void _int_(12) isr_T3(void) {
    LATDbits.LATD2 = !LATDbits.LATD2;
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}
