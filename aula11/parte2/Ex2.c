#include <detpic32.h>

typedef struct 
{ 
    char mem[100]; // Storage area 
    int nchar;   // Number of characters to be transmitted 
    int posrd;   // Position of the next character to be transmitted 
} t_buf;
volatile t_buf txbuf;

int round_div(int a,int b) { return (a + b / 2) / b; }

void putstrInt(char *s) {
    while(txbuf.nchar > 0);
    while( *s!='\0' ) {
        txbuf.mem[txbuf.nchar++] = *s;
        s++;
    }
    txbuf.posrd = 0;
    IEC1bits.U2TXIE = 1;
}

void _int_(32) isr_uart2(void){
    if(IFS1bits.U2TXIF==1){
        if(txbuf.nchar>0){
            U2TXREG = txbuf.mem[txbuf.posrd];
            txbuf.posrd++;
            txbuf.nchar--;   
        } else{
            IEC1bits.U2TXIE = 0;
        }
        IFS1bits.U2TXIF=0;
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

    // U2STAbits.UTXISEL = 1; // quando se pede interrupção do lado tx (ver manual)
    U2STAbits.URXISEL = 0; // quando se pede interrupção do lado rx (ver manual)
    IPC8bits.U2IP = 1; // prioridade da interrupção (1 a 6)
    IFS1bits.U2TXIF = 0; // limpar pedido de interrupção do lado tx
    IFS1bits.U2RXIF = 0; // limpar pedido de interrupção do lado rx
    IEC1bits.U2TXIE = 0; // ativa pedidos de interrupção na transmissão
    IEC1bits.U2RXIE = 0; // ativa pedidos de interrupção na receção
    IEC1bits.U2EIE = 0; // desativa pedidos de interrupção em caso de erro

    EnableInterrupts();

    while(1) {
        putstrInt("Test string which can be as long as you like as long as it is no longer than 100 characters\n");
    }
    return 0;
}
