
	.data
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ printInt, 6
	.equ putChar, 3
	.text
	.globl main

main:
	addi	$sp, $sp, -4			# $sp = $sp + -4
	sw		$ra, 0($sp)		# 

	li		$s0, 0		# $s0 = 0
w1:
	li		$v0, resetCoreTimer		# $v0 = resetCoreTimer
	syscall
w2:
	li		$v0, readCoreTimer		# $v0 = readCoreTimer
	syscall
	bge		$v0, 10000000, ew2	# if $v0 >= 200000 then ew2
	j		w2				# jump to w2
ew2:
	# usado para o 2
	li		$a0, 500		# $a0 = 500
	jal		delay				# jump to delay and save position to $ra

	move	$a0, $s0			# $a0 = $s0
	addi	$s0, $s0, 1			# $s0 = $s0 + 1
	li		$a1, 4		# $a1 = 4
	sll		$a1, $a1, 16			# $a1 = 4 << 16
	or		$a1, $a1, 10		# $a1 = $a1 | 10	
	li		$v0, printInt		# $v0 = printInt
	syscall

	li		$a0, '\r'		# $a0 = '\r'
	li		$v0, putChar		# $v0 = putChar
	syscall

	j		w1				# jump to while
	
ew1:
	li		$v0, 0		# $v0 = 0

	lw		$ra, 0($sp)		# 
	addi	$sp, $sp, 4			# $sp = $sp + -4
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
	
