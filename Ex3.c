#include <detpic32.h>

volatile int counter = 0;

void confLeds() {
    TRISE &= 0xFFF0;
}

void confUART2() {
    U2BRG = (20000000 / (16 * 9600)) - 1 ;
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 2;
    U2MODEbits.STSEL = 1;
    U2STAbits.URXEN = 1;
    // U2STA.UTXEN = 1;
    U2MODEbits.ON = 1;

    U2STAbits.URXISEL = 0;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 1;
    IFS1bits.U2RXIF = 0;
}

int main(void) {
    confLeds();
    confUART2();

    EnableInterrupts();

    while(1);

    return 0;

}

void _int_(32) isr_uart2() {
    printf("asdfasdf\n");
    if( IFS1bits.U2RXIF==1 ) {
        printf("asdf\n");
        char c = U2RXREG;
        if(c=='U') {
            counter++;
            printf("%d\n",counter);
        }
        IFS1bits.U2RXIF = 0;
    }
}