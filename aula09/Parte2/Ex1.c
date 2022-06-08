#include <detpic32.h>

volatile int sout = 0;

volatile int testLeds = 0x0000;

int main(void) {
    // // Timer 3 - 100Hz
    // T3CONbits.TCKPS = 2; 
    // PR3 = 49999;
    // TMR3 = 0;
    // T3CONbits.TON = 1;
    // IPC3bits.T3IP = 2;
    // IEC0bits.T3IE = 1;
    // OC1CONbits.OCM = 6;  
    // OC1CONbits.OCTSEL =0;
    // OC1RS = 12500;
    // OC1CONbits.ON = 1;

    // Timer 3 - 2Hz
    T3CONbits.TCKPS = 7; 
    PR3 = 39063;
    TMR3 = 0;
    T3CONbits.TON = 1;
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    OC1CONbits.OCM = 6;  
    OC1CONbits.OCTSEL =0;
    OC1RS = 12500;
    OC1CONbits.ON = 1;

    TRISD = TRISD | 0x0001;
    TRISE = TRISE & 0xFF00; // leds to output

    // setPWM(10);
    // setPWM(65);
    // setPWM(80);

    EnableInterrupts();

    while(1) {
        LATE = (LATE & 0xFF00) | (0x00FF & testLeds);
        putChar('\r');
        printInt(PORTD, 16 | (30<<8));
        // testLeds = (testLeds & 0x03) | (PORTDbits.PORTD11 << 4);
    }
    return 0;
}

void _int_(12) _isr_T3(void) { // send info to displays
    testLeds = (testLeds & 0x03) ^ 3;
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}

void setPWM(unsigned int dutyCicle) {
    if( dutyCicle>100 ) dutyCicle = 100;
    OC1RS = ((49999 + 1) * dutyCicle) / 100;
}
