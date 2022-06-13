#include <detpic32.h>

volatile int duty = 50;

int round_div(int a,int b) { return (a + b / 2) / b; }

int main(void) {
    /* CONFIGURATIONS */

    // Displays
    TRISB &= 0x80FF;
    TRISD &= 0xFF9F;

    // Led RC14
    TRISCbits.TRISC14 = 0;

    // Timer 2 - 1kHz
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
    OC3RS = ((PR2 + 1) * duty + 50) / 100; // duty=duty cycle pretendido=50%
    OC3CONbits.ON = 1;

    // Timer 5 - 100Hz
    T5CONbits.TON = 0;
    T5CONbits.TCKPS = 2;
    PR5 = round_div(20000000 , (100 * 4)) - 1;
    TMR5 = 0;
    IPC5bits.T5IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T5IF = 0; // limpar pedido de interrupção do temporizador x
    IEC0bits.T5IE = 1; // ativar pedidos de interrupção do temporizador x
    T5CONbits.TON = 1;

    // A/D
    TRISBbits.TRISB4 = 1; // configurar RB4 como entrada
    AD1PCFGbits.PCFG4= 0; // configurar AN4 como entrada analógica
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16; // tempo de conversão: 16 TAD (1TAD = 100ns)
    AD1CON2bits.SMPI = 4 - 1; // N conversões
    AD1CHSbits.CH0SA = 4; // AN4
    AD1CON1bits.ON = 1; // ativar conversões A/D
    IPC6bits.AD1IP = 1; // prioridade da interrupção A/D (1 a 6)
    IFS1bits.AD1IF = 0; // limpar pedido de interrupção A/D
    IEC1bits.AD1IE = 1; // ativar pedidos de interrupção A/D

    EnableInterrupts(); // o MIPS aceita pedidos de interrupção

    /* END OF CONFIGURATIONS */


    while(1) {
        AD1CON1bits.ASAM = 1;

        resetCoreTimer();
        while( readCoreTimer()<2000000 );
    }

    return 0;
}

void _int_(8) isr_timer_2(void) {
    IFS0bits.T2IF = 0; // limpar o pedido de interrupção
}

void _int_(20) isr_timer_5(void) {
    static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char flag = 0;
    flag ^= 1;
    LATDbits.LATD5 = flag;
    LATDbits.LATD6 = flag^1;

    char c = flag==0 ? display7Scodes[duty/10] : display7Scodes[duty%10];
    if( duty==100 ) {
        c = 0x5C;
        LATCbits.LATC14 = 1;
    } else {
        LATCbits.LATC14 = 0;
    }
    
    LATB = (LATB & 0x80FF) | c<<8; 

    IFS0bits.T5IF = 0; // limpar o pedido de interrupção
}

void _int_(27) isr_adc(void)
{
    int val = 4 / 2;
    int i;
    for(i = 0;i < 4;i++) {
        val += (&ADC1BUF0)[4 * i];
    }
    val /= 4; // média arredondada
    duty = (100 * val + 511) / 1023; 
    OC3RS = ((PR2 + 1) * duty + 50) / 100; // duty=duty cycle pretendido

    IFS1bits.AD1IF = 0; // limpar o pedido de interrupção
}
