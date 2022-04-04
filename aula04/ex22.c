#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
	unsigned char segment = 1;

	TRISB = (TRISB & 0x80FF);
	TRISD = (TRISD & 0xFF9F);

	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;

	while(1) {
		segment = 1;
		int i;
		for (i = 0; i < 7; i++) {
			LATB = (LATB & 0x80FF) | segment << 8;
			delay( 1000/10 );
			segment = segment << 1;
		}
		LATDbits.LATD5 = LATDbits.LATD5 ^ 1;
		LATDbits.LATD6 = LATDbits.LATD6 ^ 1;
	}

	return 0;
}