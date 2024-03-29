#include <detpic32.h>

int round_div(int a,int b) { return (a + b / 2) / b; }

void putc(char byte){
    while(U2STAbits.UTXBF==1);
    U2TXREG = byte;
}

void _int_(32) isr_uart2(void) 
{ 
    if ( IFS1bits.U2RXIF==1 ) {
        char c = U2RXREG;
        if( c=='T' ) LATCbits.LATC14 = 1;
        else if( c=='t') LATCbits.LATC14 = 0;
        putc(c);
        IFS1bits.U2RXIF = 0; 
    }
}

int main(void) {
    TRISCbits.TRISC14 = 0;

    int baud_rate = 115200;
    int ovs_factor[2] = { 16,4 };
    int ovs = 0; // 0 (x16, standard speed) ou 1 (x4, high speed)
    U2MODEbits.ON = 0; // desativa a UART
    U2MODEbits.BRGH = ovs; // configura fator de sobre amostragem (0 ou 1)
    U2BRG = round_div(20000000,ovs_factor[ovs] * baud_rate) - 1;
    U2MODEbits.PDSEL = 0; // 0 (8N), 1 (8E), 2 (8O), 3 (9N) --- ver manual
    U2MODEbits.STSEL = 0; // 0 (1 stop bits), 1 (2 stop bits) --- ver manual
    U2STAbits.UTXEN = 1; // ativa transmissão (ver nota abaixo)
    U2STAbits.URXEN = 1; // ativa receção (ver nota abaixo)
    U2MODEbits.ON = 1; // ativa UART

    U2STAbits.UTXISEL = 0; // quando se pede interrupção do lado tx (ver manual)
    U2STAbits.URXISEL = 0; // quando se pede interrupção do lado rx (ver manual)
    IPC8bits.U2IP = 1; // prioridade da interrupção (1 a 6)
    IFS1bits.U2TXIF = 0; // limpar pedido de interrupção do lado tx
    IFS1bits.U2RXIF = 0; // limpar pedido de interrupção do lado rx
    IFS1bits.U2EIF = 0; // limpar pedido de interrupção por causa de erros
    IEC1bits.U2TXIE = 0; // desativa pedidos de interrupção na transmissão
    IEC1bits.U2RXIE = 1; // ativa pedidos de interrupção na receção
    IEC1bits.U2EIE = 0; // desativa pedidos de interrupção em caso de erro

    EnableInterrupts();

    while(1);
    return 0;
}
