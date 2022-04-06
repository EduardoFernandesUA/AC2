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

unsigned char toBcd(unsigned char value) 
{ 
	return ((value / 10) << 4) + (value % 10); 
}

void send2leds(char bin) {
	LATE = (LATE & 0xFF00) | bin;
}

int main(void) {
	int counter = 0, i;
	TRISB = (TRISB & 0x80F0) | 0x000F;
	TRISD = (TRISD & 0xFF9F);
	TRISE = (TRISE & 0xFF00);

	while(1) {
		i = 0;
		do{
			send2displays(toBcd(counter));
			send2leds(toBcd(counter));
			delay(10); // 10 = 1000/100 -> 100Hz
		} while(++i < 50); // 50 = (1000/2) / (1000/100)  -> 2Hz / 100Hz
		if( (PORTB & 0x0001) == 0x1 ) counter++;
		else counter--;
		if( counter == 60 ) {
			counter = 0;
		} else if( counter == -1 ) {
			counter = 59;
		}
	}

	return 0;
}
