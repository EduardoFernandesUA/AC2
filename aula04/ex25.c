#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
	char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	TRISB = (TRISB & 0x80F0) | 0x000F;
	TRISD = (TRISD & 0xFF9F);

	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;

	while(1) {
		char dipSwitch = (LATB & 0x000F);
		char segment = display7Scodes[dipSwitch];
		LATB = (LATB & 0x80FF) | segment << 8;
	}

	return 0;
}