
	.equ	inkey, 1
	.equ	getChar, 2
	.equ	putChar, 7

	.data
	.text
	.globl main

main:

do:
	li		$v0,inkey
	syscall

	move $t0, $v0			#$a0 = $v0
if:	beq		$t0, 0, else	# if $t0 == 0 then eif
	move	$a0,$t0
	j		eif				# jump to eif
else:
	li		$a0, '.' 		# $t1 = 
eif:

	li		$v0, putChar		# $v0 = putChar
	syscall

	beq		$a0, '\n', edo	# if $a0 == '\n' then edo
	j		do				# jump to do
edo:
	li		$v0, 0		# $v0 = 0
	jr		$ra					# jump to $ra
	
