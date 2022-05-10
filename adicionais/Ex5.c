#include <detpic32.h>

void delay(int ms) {
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main() {
    TRISB = (TRISB & 0x80F0) | 0x000F;
    TRISD = (TRISD & 0xFF9F);
	TRISE = (TRISE & 0xFFF0);

    int counter = 0;
    int freq = 2 * (1 + 0); // key default -> 0
    while(1) {

        int key = inkey();

        if( key>='0' && key<='4' ) {
            freq = 2 * (1 + (key-'0'));
        } else if( key==13 ) { // [ENTER]
            printf("\r%4d, %3d Hz", counter, freq);
        }

        // printf("\r%3d", counter);
        counter = (++counter) % 100;
        delay(1000/freq);
    }
}