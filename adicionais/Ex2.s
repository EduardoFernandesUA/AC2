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
    .equ getChar, 2
	.equ putChar, 3
	.equ printInt, 6
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12
    .text
    .globl main

main:
    addiu   $sp,$sp,-4
    sw      $ra,0($sp)

    lui   	$s0, SFR_BASE_HI

    # setup dip-switch to write
    lw		$t0, TRISE($s0)
    andi	$t0, $t0, 0xFFF0
    sw		$t0, TRISE($s0)
    li      $t0,0x0000
    sw      $t0,LATE($s0)

loop:
    
    li      $v0,getChar
    syscall

if:
    blt		$v0, '0', elif	# if $v0 < '1' then eifrget
    bgt		$v0, '3', elif	# if $v0 < '1' then eifrget
    addiu   $v0, $v0, -'0'
    li      $t0,1
    sll     $v0, $t0, $v0
    sw      $v0,LATE($s0)
    j		eif				# jump to eif
elif:
    li      $t0,0x000F
    sw      $t0,LATE($s0)
    li		$v0, resetCoreTimer		# $v0 = resetCoreTimer
	syscall
delay:
	li		$v0, readCoreTimer		# $v0 = readCoreTimer
	syscall
	bge		$v0, 20000000, edelay	# if $v0 >= 200000 then ew2
	j		delay				# jump to w2
edelay:
    li      $t0,0x0000
    sw      $t0,LATE($s0)
eif:
    j		loop				# jump to loop
    
    li		$v0, 0		# $v0 = 0

    lw      $ra,0($sp)
    addiu   $sp,$sp,4
    jr		$ra					# jump to $ra
    

    

