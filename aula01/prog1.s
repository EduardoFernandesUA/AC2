#	int main(void)
#	{
#		printStr("AC2 - Aulas praticas\n");
#		return 0;
#	}

	.equ	PRINT_STR, 8
	
	.data
msg:	.asciiz	"AC2 - Aulas praticas\n"
	.text
	.globl main
main:	
	la		$a0, msg		# $a0 = msg
	li		$v0, PRINT_STR	# $v0 = PRINT_STR
	syscall					# printStr("AC2 - Aulas praticas");
	li		$v0, 0			# $v0 = 0
	jr		$ra				# jump to $ra
	
	