#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
	TRISE = (TRISE & 0xFFF0) | 0x0000;

	unsigned int n = 0;
	while(1) {
		delay( 1000/4 );
		LATE = (LATE & 0xFFF0) | n;
		n = (n+1) & 0x000F;
	}

	return 0;
}