#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int main(void) {
	//configure port rc14 as output
	TRISCbits.TRISC14 = 0;

	int n = 0x0;
	while(1) {
		delay(500);
		n = n==0x0 ? 0x1 : 0x0;
		LATCbits.LATC14 = ;
	}

	return 0;
}
