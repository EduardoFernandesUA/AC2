#include <detpic32.h>

int round_div(int a,int b) { return (a + b / 2) / b; }

int main(void) {
    /* CONFIGURATIONS */

    // Timer 1 - 1kHz
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 0;
    PR2 = round_div(20000000 , (1000 * 1)) - 1;
    TMR2 = 0;
    IPC2bits.T2IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T2IF = 0; // limpar pedido de interrupção do temporizador x
    IEC0bits.T2IE = 1; // ativar pedidos de interrupção do temporizador x
    T2CONbits.TON = 1;

    // PWM from Timer 2 to OC3
    OC3CONbits.OCM = 6;
    OC3CONbits.OCTSEL = 0; // 0=temporizador 2, 1=temporizador 3
    OC3RS = ((PR2 + 1) * 50 + 50) / 100; // duty=duty cycle pretendido=50%
    OC3CONbits.ON = 1;

    // A/D
    TRISBbits.TRISB4 = 1; // configurar RB4 como entrada
    AD1PCFGbits.PCFG4= 0; // configurar AN4 como entrada analógica
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16; // tempo de conversão: 16 TAD (1TAD = 100ns)
    AD1CON2bits.SMPI = 4 - 1; // N conversões
    AD1CHSbits.CH0SA = 4; // AN4
    AD1CON1bits.ON = 1; // ativar conversões A/D

    /* END OF CONFIGURATIONS */


    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );

        int val = 4 / 2;
        int i;
        for(i = 0;i < 4;i++) {
            val += (&ADC1BUF0)[4 * i];
        }
        val /= 4; // média arredondada
        int duty = (100 * val + 511) / 1023; 
        OC3RS = ((PR2 + 1) * duty + 50) / 100; // duty=duty cycle pretendido

        resetCoreTimer();
        while( readCoreTimer()<20000000/10 );
    }

    return 0;
}
