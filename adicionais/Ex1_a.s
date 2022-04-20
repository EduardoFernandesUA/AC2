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
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ printInt, 6
	.equ putChar, 3
    .text
    .globl main

main:
    lui   	$s0, SFR_BASE_HI

    # setup dip-switch to write
    lw		$s1, TRISB($s0)
    ori		$s1, $s1, 0x000F
    sw		$s1, TRISB($s0)

    # setup dip-switch to write
    lw		$s1, TRISE($s0)
    andi	$s1, $s1, 0xFFF0
    sw		$s1, TRISE($s0)

loop:
    lw		$t0, PORTB($s0)		# 
    andi	$t0, $t0, 0x000F			# $t0 = $t0 & 0x000F
    lw		$t1, LATE($s0)		# 
    andi	$t1, $t1, 0xFFF0			# $t0 1 $t01& 0xFFF0
    or		$t0, $t0, $t1			# $t0 = $t0 | $t1
    sw		$t0, LATE($s0)		# 
    

    j		loop				# jump to loop
    
    li		$v0, 0		# $v0 = 0
    jr		$ra					# jump to $ra
    

    

