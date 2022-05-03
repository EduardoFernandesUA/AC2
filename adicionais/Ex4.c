#include <detpic32.h>

void delay(unsigned int ms) {
	int K = 20000;
	resetCoreTimer(); 
	while(readCoreTimer() < K * ms);
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

int main() {
	TRISB = (TRISB & 0x80F0) | 0x000F;
    TRISD = (TRISD & 0xFF9F);
	TRISE = (TRISE & 0xFFF0);

    int key;
    int i = 0;
    int on = 0;
    char v = 0x00;
    LATE = (LATE & 0xFFF0);
	while(1) {
        delay(10);

        if( on ) send2displays(v);
		else LATD = (LATD & 0xFF9F) | 0x0000;

        if(i>=0) {
            LATE = (LATE | 0x000F);
            i -= 10;
            if( i<=0 ) {
                LATE = (LATE & 0xFFF0);
                on = 0;
            }
            continue;
        }

        key = inkey();
        printf("\r%4c", key);
        if( key>='0' && key<='3' ) {
            LATE = (LATE & 0xFFF0) | (0x0001 << (key-'0'));
            v = key-'0';
            on = 1;
        } else if( key!=0 ) {
            i = 1000;
            v = 0xFF;
            on = 1;
        }
	}
}