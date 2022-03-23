
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
	li		$s3, 0 # int v = 0;

	lui   	$s0, SFR_BASE_HI

	# RB0 -> to write
	lw  	$s1, TRISE($s0)
	andi	$s1, $s1, 0xFFF0
	sw  	$s1, TRISE($s0)

	# RD0 -> to write
	lw  	$s1, TRISD($s0)
	andi	$s1, $s1, 0xFFF0
	sw  	$s1, TRISD($s0)

loop:
	lw		$s2, LATE($s0)
	andi	$s2, $s2, 0xFFF0
	or		$s2, $s2, $s3
	sw		$s2, LATE($s0)

	lw		$s2, LATD($s0)
	andi	$s2, $s2, 0xFFF0
	or		$s2, $s2, $s3
	sw		$s2, LATD($s0)

	li		$a0,  1000
	jal		delay

	addi	$s3, $s3, 0x0001
	andi	$s3, $s3, 0x000F

	j		loop				# jump to loop
	
	jr		$ra




##############################
delay:
	move 	$t0, $a0		# $t0 = $a0
	li		$t1, 20000		# $t1 = 20000
	mul		$t0, $t0, $t1		
	li		$v0, resetCoreTimer		# $v0 = resetCoreTimer
	syscall
delay_w:
	li		$v0, readCoreTimer		# $v0 = readCoreTimer
	syscall
	bge		$v0, $t0, delay_ew	# if $t0 >= $v0 then delay_ew
	j		delay_w				# jump to delay_w
delay_ew:	
	li		$v0, 0		# $v0 = 0
	jr		$ra					# jump to $ra
	