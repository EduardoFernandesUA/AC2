#include <detpic32.h>

int round_div(int a,int b) { return (a + b / 2) / b; }

void putc(char byte2send) {
    while( U2STAbits.UTXBF==1 );
    U2TXREG = byte2send;
}

void putstr(char *str) {
    while( *str!='\0' ) {
        putc(*str);
        str++;
    }
}

void confDisplays() {
    TRISB &= 0x80FF;
    TRISD &= 0xFF9F;
}

void confSwitch() {
    TRISB |= 0x000F;
}

void confUART2() {
    int baud_rate = 1200;
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)
    U2BRG = round_div(20000000,ovs_factor[ovs] * baud_rate) - 1;
    U2MODEbits.PDSEL = 0; // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual
    U2MODEbits.STSEL = 0; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 0; // ativa receção (ver nota abaixo)
    U2MODEbits.ON = 1; // ativa UART
}

int main(void) {
    // Configurations
    confDisplays();
    confSwitch();
    confUART2();

    EnableInterrupts();

    while(1) {
        putstr("RB30=");
        int i;
        for (i = 0; i < 4; i++)
            putc( (PORTB>>i)&1 + '0' );
        putc('\n');
        resetCoreTimer();
        while( readCoreTimer()<10000000 );
    }

    return 0;
}
