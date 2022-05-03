
#include <detpic32.h>

int delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

int printValues(int a, int b, int c) {
	printInt(a, 0x0005000A);
	putChar(' ');
	printInt(b, 0x0005000A);
	putChar(' ');
	printInt(c, 0x0005000A);
	putChar('\r');
}

int main(void) {
	int counting = 1; // boolean
	int key;
	int cnt1 = 0, cnt5 = 0, cnt10 = 0;
	while(1) {
		key = inkey();

		if( key=='a' ) delay(50);
		else delay(100);

		if(key=='s') counting = 0;
		else if(key=='r') counting = 1;

		if( counting ) {
			cnt10 += 1;
			if( cnt10% 2==0 ) cnt5 += 1;
			if( cnt10%10==0 ) cnt1 += 1;
			printValues(cnt10, cnt5, cnt1);
		}
	}
	return 0;
}
