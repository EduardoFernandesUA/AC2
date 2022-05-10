#include <detpic32.h>

void delay(int i) {
    resetCoreTimer();
    while(readCoreTimer() < i);
}

int main() {
    TRISB = TRISB | 0x0004;
    TRISE = TRISE & 0xFFC0;

    int i = 0;
    int on = 0;
    while(1) {
        on = PORTB & 0x0004;

        LATE = (LATE & 0xFFC0) | (0x1 << i);
        i = (++i) % 6;

        if( on ) delay( 20000000/7 );
        else delay( 20000000/3 );
    }
} 1000/3 = 333 * 3 = 999