
	.data
	.equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area 
	.equ TRISB, 0x6040
	.equ TRISD, 0x60c0
	.equ TRISE, 0x6100   # TRISE address is 0xBF886100 
	.equ PORTB, 0x6050
	.equ PORTD, 0x60D0
	.equ PORTE, 0x6110   # PORTE address is 0xBF886110 
	.equ LATD, 0x60E0
	.equ LATE, 0x6120   # LATE  address is 0xBF886120 
	.text
	.globl main

main:
	lui   	$t0, SFR_BASE_HI

	# RB[0-3] -> to read
	lw  	$t1, TRISB($t0) 
	ori		$t1, $t1, 0x000F
	sw  	$t1, TRISB($t0)

	# RE[0-3] -> to write
	lw  	$t1, TRISE($t0)
	andi	$t1, $t1, 0xFFF0
	sw  	$t1, TRISE($t0)

loop:
	lw 		$t1, PORTB($t0)
	andi	$t1, $t1, 0x000F

	xor		$t1, $t1, 0x009

	lw		$t2, LATE($t0)
	andi	$t2, $t2, 0xFFF0
	or		$t2, $t2, $t1
	sw		$t2, LATE($t0)
	j		loop				# jump to loop
	
	jr		$ra

