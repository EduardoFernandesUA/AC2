#include <detpic32.h>

int main(void) {
    TRISE = (TRISE & 0xFFF0);
    LATE = LATE & 0xFFF0;
    LATE = LATE | 0x0003;
    while(1) {
        char c = getChar();
        if( c>='0' && c<='53' ) {
            LATE = (LATE & 0xFFF0) | (0x1 << (c-'0'));
        } else if( c!=0 ) {
            LATE = LATE | 0x000F;
            resetCoreTimer();
            while(readCoreTimer() < 20000000);
            LATE = LATE & 0xFFF0;
        }
    }
}