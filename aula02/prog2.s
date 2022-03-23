
	.data
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ printInt, 6
	.equ putChar, 3
	.text
	.globl main

# Mapa de Registos
# s0 - counting
# s1 - cnt10
# s2 - cnt5
# s3 - cnt1
# s4 - key

main:
	li		$s0, 1		# $s0 = 1
	li		$s1, 0
	li		$s2, 0
	li		$s3, 0

w1:

if1:	
	bne		$s4, 'a', eif1	# if $s4 == 'a' then eif1
	li		$a0, 50		# $a0 = 50
	j		eif1				# jump to eif1
elif1:	
	li		$a0, 100		# $a0 = 100
eif1:
	jal		delay				# jump to delay and save position to $ra

if2:	
	bne		$s4, 's', eif2
	li		$s0, 0		
	j		eif2				
elif2:	
	bne		$s4, 'r', eif2
	li		$s0, 1		
eif2:

if3:
	bne		$s0, 1, eif3	# if $s0 != 1 then eif3
	addi	$s1, $s1, 1			# $s1 = $s1 + 1
	li		$t0,2
	rem		$t0,$s1,$t0
if4:
	bne		$t0, 0, eif4	# if $t0 != 0 then eif4
	addi	$s2, $s2, 1			# $s2 = $s2 + 1
eif4:
	li		$t0,5
	rem		$t0,$s1,$t0
if5:
	bne		$t0, 0, eif5	# if $t0 != 0 then eif4
	addi	$s3, $s3, 1			# $s2 = $s2 + 1
eif4:

	move 	$a0, $s1		# $a0 = $s1
	li		$a1,0x0005000A
	li		$v0, printInt		# $v0 = printInt
	syscall		

	li		$a0, ' '		# $a0 = ' '
	li		$v0, putChar		# $v0 = putChar
	syscall

	move 	$a0, $s2		# $a0 = $s1
	li		$a1,0x0005000A
	li		$v0, printInt		# $v0 = printInt
	syscall		

	li		$a0, ' '		# $a0 = ' '
	li		$v0, putChar		# $v0 = putChar
	syscall

	move 	$a0, $s3		# $a0 = $s1
	li		$a1,0x0005000A
	li		$v0, printInt		# $v0 = printInt
	syscall		

	li		$a0, '\r'		# $a0 = ' '
	li		$v0, putChar		# $v0 = putChar
	syscall
eif5:

	j		w1				# jump to w1
	
ew1:
	li		$v0, 0		# $v0 = 0
	jr		$ra					# jump to $ra
	
	




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
	
