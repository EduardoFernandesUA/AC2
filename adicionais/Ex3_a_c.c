#include <detpic32.h>

int main() {
	TRISB = (TRISB | 0x000F);
	TRISE = (TRISE & 0xFF00);

	while(1) {
		int twoComplement = PORTBbits.RB0 | PORTBbits.RB1 << 1 | PORTBbits.RB2 << 2 | PORTBbits.RB3 << 3;
		twoComplement = ((twoComplement ^ 0xFF) & 0x0F) + 1;
		LATEbits.LATE0 = twoComplement & 0x01;
		LATEbits.LATE1 = (twoComplement & 0x02) >> 1;
		LATEbits.LATE2 = (twoComplement & 0x04) >> 2;
		LATEbits.LATE3 = (twoComplement & 0x08) >> 3;

		LATEbits.LATE7 = PORTBbits.RB3;
		LATEbits.LATE6 = PORTBbits.RB2;
		LATEbits.LATE5 = PORTBbits.RB1;
		LATEbits.LATE4 = PORTBbits.RB0;

	}
}