.globl sys_write
sys_write:
	push	%rbp
	mov	%rsp, %rbp

	push	%rbx
	push	%rdx

	mov	%rdi, %rbx
	mov	%rsi, %rcx
	mov	%rdx, %rdx
	movl	$0x04, %eax
	int 	$0x80

	pop	%rdx
	pop	%rbx

	mov	%rbp, %rsp
	pop	%rbp
	ret

.globl sys_read
sys_read:
	push	%rbp
	mov	%rsp, %rbp

	push	%rbx
	push	%rdx

	mov	$0x0, %rax
	mov	%rdi, %rbx
	mov	%rsi, %rcx
	mov	%rdx, %rdx
	movl	$0x03, %eax
	int 	$0x80

	pop	%rdx
	pop	%rbx

	mov	%rbp, %rsp
	pop	%rbp
	ret
	

.globl sys_exit
sys_exit:
	push 	%rbp
	mov	%rsp, %rbp

	mov	0x10(%rbp), %rbx
	mov	$0x01, %rax
	int 	$0x80

forever:
	mov	%rax, %rax
	jmp 	forever
