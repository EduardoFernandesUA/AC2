#include <detpic32.h>

void delay(int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(unsigned char value) {
	static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	static char displayFlag = 0;

	LATDbits.LATD5 = displayFlag ^ 1;
	LATDbits.LATD6 = displayFlag;

	char value_normalized = (value >> displayFlag*4) & 0x0F;
	char segment = display7Scodes[ value_normalized ];
	LATB = (LATB & 0x80FF) | segment << 8;

	displayFlag = displayFlag ^ 1;
}

unsigned char toBcd(unsigned char value) 
{ 
	return ((value / 10) << 4) + (value % 10); 
}

int main() {
    TRISB = (TRISB & 0x80F0) | 0x000F;
    TRISD = (TRISD & 0xFF9F);
	TRISE = (TRISE & 0xFFF0);

    int counter = 0;
    int freq = 2; // 2 * (1 + 0)
    int baseRefresh = 1000/100; // 100Hz
    int cFrames = 0;
    char displayValue = 0;
    while(1) {

        int key = inkey();

        if( key>='0' && key<='4' ) {
            freq = 2 * (1 + key);
        } else if( key==13 ) { // [ENTER]
            displayValue = counter;
        }
        
        printf("\r%4d, %3d Hz", counter, freq);
        if( cFrames%2==0 ) send2displays( toBcd(displayValue) ); // 50Hz
        delay(baseRefresh);
        cFrames++;

        if( cFrames/(100/freq) >= 1 ) {
            counter = (++counter) % 100;
            cFrames = 0;
        }
    }
}

// 2 4 6 8 10