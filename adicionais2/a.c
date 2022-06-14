#include <detpic32.h>

int round_div(int a,int b) { return (a + b / 2) / b; }
static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};

volatile int counter = 0;
int main(void) {
    // PORTS Configuration
    TRISB = (TRISB & 0x80FF);
    TRISD = (TRISD & 0xFF9F);

    // T1
    T1CONbits.TON = 0;
    T1CONbits.TCKPS = 2;  // 31.5 --> 64
    PR1 = round_div(20000000, (10*64)) - 1;
    TMR1 = 0;
    IPC1bits.T1IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T1IF = 0; // limpar pedido de interrupção do temporizador 1
    IEC0bits.T1IE = 1; // ativar pedidos de interrupção do temporizador 1
    T1CONbits.TON = 1;
    // T2
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 3;  // 6 --> 8
    PR2 = round_div(20000000, (50*8)) - 1;
    TMR2 = 0;
    IPC2bits.T2IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T2IF = 0; // limpar pedido de interrupção do temporizador 2
    IEC0bits.T2IE = 1; // ativar pedidos de interrupção do temporizador 2
    T2CONbits.TON = 1;

    EnableInterrupts();

    while(1);

    return 0;
}

void _int_(4) isr_timer_1(void) {
    counter = (counter + 1) % 100;
    printInt(counter, 16 | 2 << 16);
    putChar('\n');
    IFS0bits.T1IF = 0; // reset
}

void _int_(8) isr_timer_2(void) {
    static char flag = 0;
    flag = flag^1;
    LATDbits.LATD5 = flag;
    LATDbits.LATD6 = flag^1;

    if( flag==0 ) {
        LATB = (LATB & 0x80FF) | display7Scodes[counter / 10] << 8; 
    } else {
        LATB = (LATB & 0x80FF) | display7Scodes[counter % 10] << 8;
    }

    IFS0bits.T2IF = 0; // reset
}