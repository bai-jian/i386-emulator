.globl start

start:
	movl  $0, %ebp
	movl  $(1<<27), %esp

	jmp init
