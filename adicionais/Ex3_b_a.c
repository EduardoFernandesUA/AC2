#include <detpic32.h>

int main() {
	TRISB = (TRISB | 0x000F);
	TRISE = (TRISE & 0xFFF0);

	while(1) {
		LATE = (LATE & 0xFFF0) | (PORTB & 0x000F);
	}
}