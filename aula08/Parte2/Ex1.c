#include <detpic32.h>

// volatile int count = 0;

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
    TRISEbits.TRISE0 = 0;
    TRISDbits.TRISD8 = 1;

    LATEbits.LATE0 = 0;

    while(1) {
        if( PORTDbits.RD8==0 ) {
            printf("\r1");
            LATEbits.LATE0 = 1;
            delay(3000);
            LATEbits.LATE0 = 0;
        }
    }

    return 0;
}
