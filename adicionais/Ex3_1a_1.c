// PROGRAM NOT TESTED, MAY NOT WORK

#include <detpic32.h>

void main() {
	TRISB = (TRISB | 0x000F);
	TRISE = (TRISE & 0xFF00);

	while(true) {

		// a)
		// LATEbits.LATE0 = PORTBbits.PORTB0;
		// LATEbits.LATE1 = PORTBbits.PORTB1;
		// LATEbits.LATE2 = PORTBbits.PORTB2;
		// LATEbits.LATE3 = PORTBbits.PORTB3;

		// b)
		// LATEbits.LATE3 = PORTBbits.PORTB0;
		// LATEbits.LATE2 = PORTBbits.PORTB1;
		// LATEbits.LATE1 = PORTBbits.PORTB2;
		// LATEbits.LATE0 = PORTBbits.PORTB3;


		// c)
		LATEbits.LATE0 = PORTBbits.PORTB0;
		LATEbits.LATE1 = PORTBbits.PORTB1;
		LATEbits.LATE2 = PORTBbits.PORTB2;
		LATEbits.LATE3 = PORTBbits.PORTB3;
		LATEbits.LATE4 = PORTBbits.PORTB3;
		LATEbits.LATE5 = PORTBbits.PORTB2;
		LATEbits.LATE6 = PORTBbits.PORTB1;
		LATEbits.LATE7 = PORTBbits.PORTB0;
	}
}