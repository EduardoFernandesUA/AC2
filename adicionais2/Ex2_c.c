#include <detpic32.h>

static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
volatile char counter = 0;

int round_div(int a,int b) { return (a + b / 2) / b; }

int main(void) {
    /* CONFIGURATIONS */

    // Both displays
    TRISB &= 0x80FF;
    TRISD &= 0xFF9F;

    // Timer 1 - 10Hz
    T1CONbits.TON = 0;
    T1CONbits.TCKPS = 2;
    PR1 = round_div(20000000 , (10 * 64)) - 1;
    TMR1 = 0;
    IPC1bits.T1IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T1IF = 0; // limpar pedido de interrupção do temporizador x
    IEC0bits.T1IE = 1; // ativar pedidos de interrupção do temporizador x
    T1CONbits.TON = 1;

    // Timer 2 - 50Hz
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 3;
    PR2 = round_div(20000000 , (50 * 8)) - 1;
    TMR2 = 0;
    IPC2bits.T2IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T2IF = 0; // limpar pedido de interrupção do temporizador x
    IEC0bits.T2IE = 1; // ativar pedidos de interrupção do temporizador x
    T2CONbits.TON = 1;

    TRISBbits.TRISB4 = 1; // configurar RB4 como entrada
    AD1PCFGbits.PCFG4= 0; // configurar AN4 como entrada analógica
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16; // tempo de conversão: 16 TAD (1TAD = 100ns)
    AD1CON2bits.SMPI = 10 - 1; // N conversões
    AD1CHSbits.CH0SA = 4; // AN4
    AD1CON1bits.ON = 1; // ativar conversões A/D


    /* END OF CONFIGURATIONS */
    int N = 10;
    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );
        int val = N / 2;
        int i = 0;
        for(i = 0;i < N;i++) {
            val += (&ADC1BUF0)[4 * i];
        }
        val /= N;
        int freq = 1 + val/127;
        PR2 = round_div(20000000 , (freq * 8)) - 1;
    }

    return 0;
}

void _int_(4) isr_timer_1(void)
{
    counter++;
    counter = counter % 100;
    IFS0bits.T1IF = 0; // limpar o pedido de interrupção
}

void _int_(8) isr_timer_2(void)
{
    static char flag = 0;
    flag = flag^1;
    LATDbits.LATD5 = flag;
    LATDbits.LATD6 = flag^1;

    if( flag==0 ) {
        LATB = (LATB & 0x80FF) | display7Scodes[counter%10]<<8; 
    } else {
        LATB = (LATB & 0x80FF) | display7Scodes[counter/10]<<8;
    }

    IFS0bits.T2IF = 0; // limpar o pedido de interrupção
}
