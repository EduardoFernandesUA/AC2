#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
}

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	static char displayFlag = 0;

	LATDbits.LATD5 = displayFlag;
	LATDbits.LATD6 = displayFlag ^ 1;

	char value_normalized = (value >> displayFlag*4) & 0x0F;
	char segment = display7Scodes[ value_normalized ];
	LATB = (LATB & 0x80FF) | segment << 8;

	displayFlag = displayFlag ^ 1;
}

unsigned char toBcd(unsigned char value) 
{ 
	return ((value / 10) << 4) + (value % 10); 
}

void send2leds(char bin) {
	LATE = (LATE & 0xFF00) | bin;
}

void blink(char n, unsigned int delay) {
	int on = 0, i=0;
	do{
		if(on) send2displays(toBcd(n));
		else LATD = (LATD & 0xFF9F) | 0x0000;

		if( i%delay/10 == 0 ) on = on ^ 1;

		delay(10);
	} while( ++i < 100*5 ); // 5sec or 2sec
}

int main(void) {
	int counter = 0, i;
	TRISB = (TRISB & 0x80F0) | 0x000F;
	TRISD = (TRISD & 0xFF9F);
	TRISE = (TRISE & 0xFF00);

	while(1) {
		int step = PORTB & 0x01; // step: 1 -> ascending, 0 -> descending

		i = 0;
		do{
			send2displays(toBcd(counter));
			send2leds(toBcd(counter));
			delay(10); // 10 = 1000/100 -> 100Hz
		} while( step==1 ? ++i < 20 : ++i < 50 ); // 50 = (1000/2) / (1000/100)  -> 2Hz / 100Hz

		if( step==1 ) {
			counter++;
			if( counter>=59 ) {
				blink(59, 200);
				counter = 0;
			}
		}
		else {
			counter--;
			if( counter<=0 ) {
				blink(0, 500);
				counter = 59;
			}
		}
	}

	return 0;
}
