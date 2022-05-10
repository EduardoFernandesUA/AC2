

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
    addiu   $sp,$sp,-4
    sw      $ra,0($sp)

    lui     $s0,SFR_BASE_HI

    lw      $s1,TRISE($s0)
    andi    $s1,$s1,0xFFF0
    sw      $s1,TRISE($s0)

    li      $s2,0 # counter = 0;
    li      $s3,2 # freq = 2 * (1+0);
    li      $s4,0 # key = 0

loop:

    li      $v0,inkey
    syscall
    move    $s4,$v0

if: 
    blt     $s4,'0',eif
    bgt     $s4,'4',eif
    subi	$s4,$s4,'0'
    addiu   $s4,$s4,1
    mul	    $s3,$t4,2			# $s4 * $t1 = Hi and Lo registers    
elif:
    bne		$s4, 13, eif	# if $s4 != 13 then eif
    move    $a0,$s2
    li      $v0,printInt
    syscall
eif:
    addiu   $s2,$s2,1
    remi    $s2,$s2,100

    divi    $a0,$t0,$s3
    jal     delay

    j       loop



    lw      $ra,0($sp)
    addiu   $sp,$sp,4

    li      $v0,0
    jr      $ra





###########################
# $a0: ms
delay:

delay_loop:

    j		delay_loop				# jump to delay_loop
e_delay_loop:
    li      $v0,0
    jr		$ra					# jump to $ra
    