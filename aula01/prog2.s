# Mapa de Registos
# $t0: c

	.equ	getChar,2
	.equ	putChar,3
	.data
	.text
	.globl main

main:
do:
	li		$v0,getChar
	syscall

	move $a0, $v0				#$a0 = $v0
	li		$v0, putChar		# $v0 = putChar
	syscall

	beq		$a0, '\n', edo		# if $a0 == '\n' then edo
	j		do					# jump to do

edo:
	li		$v0, 0				# $v0 = 0
	jr		$ra					# jump to $ra
	
