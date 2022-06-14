#include <detpic32.h>

volatile int n = -1;

int round_div(int a,int b) { return (a + b / 2) / b; }

void refreshDisplays() {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char sel = 0;

    if( n<0 ) {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
        return;
    };

    LATDbits.LATD5 = sel;
    LATDbits.LATD6 = sel^1;
    sel = sel^1;

    char c = sel==0 ? display7Scodes[ n%16 ] : display7Scodes[ n/16 ];

	LATB = (LATB & 0x80FF) | c << 8;
}

int main(void) {
    /* CONFIGURATIONS */

    // leds<RE3,RE2,RE1,RE0> - pag.162
    TRISE &= 0xFFF0;

    // displays
    TRISB &= 0x80FF;
    TRISD &= 0xFF9F;

    // Timer 2 - 100Hz
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 2;
    PR2 = round_div(20000000 , (100 * 4)) - 1;
    TMR2 = 0;
    IPC2bits.T2IP = 1; // prioridade da interrupção (1 a 6)
    IFS0bits.T2IF = 0; // limpar pedido de interrupção do temporizador x
    IEC0bits.T2IE = 1; // ativar pedidos de interrupção do temporizador x
    T2CONbits.TON = 1;

    EnableInterrupts();
    
    /* END OF CONFIGURATIONS */ 

    LATE &= 0xFFF0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    while(1){
        int c = inkey()-'0';
        if ( c>=0 && c<=3 ) {
            LATE = (LATE & 0xFFF0) | (0x1 << (c));
            n = c;
        } else if( c+'0'!=0 ) {
            LATE |= 0x000F;
            n = 0x00FF;
            resetCoreTimer();
            while(readCoreTimer()<20000000);
            LATE &= 0xFFF0;
            n = -1;
        }
    }

    return 0;
}

void _int_(8) isr_timer_2(void)
{
    refreshDisplays();
    IFS0bits.T2IF = 0; // limpar o pedido de interrupção
}
