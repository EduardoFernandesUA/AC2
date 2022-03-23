
	.data
	.equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area 
	.equ TRISB, 0x6040
	.equ TRISD, 0x60c0
	.equ PORTB, 0x6050
	.equ PORTD, 0x60D0
	.equ TRISE, 0x6100   # TRISE address is 0xBF886100 
	.equ PORTE, 0x6110   # PORTE address is 0xBF886110 
	.equ LATE, 0x6120   # LATE  address is 0xBF886120 
	.equ LATD, 0x60E0
	.text
	.globl main

main:
	lui   	$t0, SFR_BASE_HI #  
	lw  	$t1, TRISB($t0)  # READ  (Mem_addr = 0xBF880000 + 0x6040) 
	ori		$t1, $t1, 0x0001
	sw  	$t1, TRISB($t0)  # WRITE (Write TRISE register)

	lw  	$t1, TRISE($t0)  # READ  (Mem_addr = 0xBF880000 + 0x6040) 
	andi	$t1, $t1, 0xFFFE
	sw  	$t1, TRISE($t0)  # WRITE (Write TRISE register)

	lw  	$t1, TRISD($t0)  
	ori		$t1, $t1, 0x0100
	sw  	$t1, TRISD($t0)  

loop:
	# lw 		$t1, PORTB($t0)
	# andi	$t1, $t1, 0x0001

	lw 		$t1, PORTD($t0)
	andi	$t1, $t1, 0x0100

	# xori	$t1, $t1, 0x0001			# $t1 = $t1 ^ 0x0001

	lw		$t2, LATE($t0)
	andi	$t2, $t2, 0xFFFE
	srl		$t1, $t1, 8
	or		$t2, $t2, $t1
	sw		$t2, LATE($t0)
	j		loop				# jump to loop
	
	jr		$ra

