.globl test
test:
	push	%rbp
	mov	%rsp, %rbp

	push	%rdi
	push	%rsi
	push	%rdx

	mov	$0x01, %rdi
	xor	%rsi, %rsi
	mov	%rsi, %rdx

	call	sys_write

	pop	%rdx
	pop	%rsi
	pop	%rdi

	mov	%rbp, %rsp
	pop	%rbp

	ret

.globl sys_write
sys_write:
	push	%rbp
	mov	%rsp, %rbp

# Save the parameters
	push	%rdi
	push	%rsi
	push	%rdx

	push	%rcx
	push	%r11

# setup the system call
	mov	$4, %rax
	int 	$0x80

	pop	%r11
	pop	%rcx

# Restore the variables	
	pop	%rdx
	pop	%rsi
	pop	%rdi

# Leave
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
