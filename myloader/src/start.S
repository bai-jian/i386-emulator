.globl start

start:
	movl  $0, %ebp
	movl  $(128<<20), $esp

	jmp init
