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

    lui   	$s0, SFR_BASE_HI

    # setup dip-switch to read
    lw		$s1, TRISB($s0)
    ori		$s1, $s1, 0x000F
    sw		$s1, TRISB($s0)

    # setup leds to write
    lw		$s1, TRISE($s0)
    andi	$s1, $s1, 0xFF00
    sw		$s1, TRISE($s0)

loop:
    lw		$s2, PORTB($s0)		# 
    andi	$s2, $s2, 0x000F    # $s2 = value in dip-switch
    
    # s3 -> binary to send to leds
    ori		$s3, $s2, 0		# $t0 = $t0 | $t1

    move    $a0,$s2
    jal     reverseBits
    sll     $t0,$v0,4
    or      $s3,$s3,$t0

    lw		$t0, LATE($s0)		# 
    andi	$t0, $t0, 0xFF00	# $t0 1 $t01& 0xFFF0
    or     $t0, $t0, $s3
    sw		$t0, LATE($s0)		# 

    j		loop				# jump to loop
    
    li		$v0, 0		# $v0 = 0
    
    lw		$ra, 0($sp)		# 
    addiu   $sp,$sp,4
    jr		$ra					# jump to $ra
    

    
############################
reverseBits:
    # n = $a0
    li $t2,0 # i = 0
    li  $t0,0
reverseBits_while:
    bge $t2,4,reverseBits_ewhile
    sll $t0,$t0,1
    andi $t1,$a0,1 # get less significant bit
    or  $t0,$t0,$t1
    srl $a0,$a0,1
    addiu $t2,$t2,1
    j reverseBits_while
reverseBits_ewhile:
    move $v0,$t0
    jr      $ra