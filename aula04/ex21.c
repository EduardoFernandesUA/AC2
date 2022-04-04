#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
	TRISB = (TRISB & 0x80FF) | 0x0000;
	TRISD = (TRISD & 0xFF9F) | 0x0000;

	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;

	char c;
	while(1) {
		c = getChar();
		if( (c>='a' && c<='g') || (c>='A' && c<='G') ) {
			c -= c<='g' ? 'a' : 'A';
			LATB = (LATB & 0x80FF) | 0x1 << (c + 8);
		}
	}

	return 0;
}