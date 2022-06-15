#include <detpic32.h>

void confTimer2() {
    T2CONbits.TCKPS = 2; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR2 = 33333; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; 

    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation
    OC2RS = (33333*25)/100; // Ton constant
    OC2CONbits.ON = 1; // Enable OC1 module
}

void confSwitches() {
    TRISB |= 0x0009;
}

int main(void) {
    confTimer2();
    confSwitches();

    while(1) {
        char s1 = PORTB & 0x0001;
        char s4 = (PORTB & 0x0008) >> 3;
        if( s1==1 && s4==0 ) OC2RS = (33333*25)/100;
        if( s1==0 && s4==1 ) OC2RS = (33333*70)/100;
        resetCoreTimer();
        while( readCoreTimer()<5000 );
    }
}
