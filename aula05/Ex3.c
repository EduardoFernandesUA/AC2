#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	static char displayFlag = 0;
	char segment;

	if( displayFlag==0 ) {
		// selecy display high
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;

		// send digit_high (dh) to display
		segment = display7Scodes[value >> 4];
		LATB = (LATB & 0x80FF) | segment << 8;
	} else {
		// selecy display low
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;

		// send digit_high (dh) to display
		segment = display7Scodes[value & 0x0F];
		LATB = (LATB & 0x80FF) | segment << 8;
	}

	displayFlag = displayFlag ^ 1;
}

int main(void) {
	TRISB = (TRISB & 0x80FF);
	TRISD = (TRISD & 0xFF9F);

	while(1) {
		send2displays(0x15);
		delay(1000/100);
	}

	return 0;
}
