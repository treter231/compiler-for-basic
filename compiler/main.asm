section .text
	global	main
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 48
	mov		rdx, 2
	mov		[rbp-8], rdx
.L5:
	mov		rdx, [rbp-8]
	mov		rcx, 3
	cmp		rdx, rcx
	jge		.L105
	mov		rdx, [rbp-8]
	mov		rcx, 1
	add		rdx, rcx
	mov		[rbp-8], rdx

	push	rdi
	push	rax
	push	rsi

	mov		rdi, formInt
	mov		rax, [rbp-8]
	mov		rsi, rax
	xor		rax, rax
	call	printf
	pop		rsi
	pop		rax
	pop		rdi

	jmp		.L5
.L105:
	mov		rdx, 2
	mov		[rbp-24], rdx
.L7:
	mov		rdx, [rbp-24]
	mov		rcx, 3
	cmp		rdx, rcx
	jge		.L107
	mov		rdx, 1
	mov		[rbp-16], rdx

	push	rdi
	push	rax
	push	rsi

	mov		rdi, formInt
	mov		rax, [rbp-16]
	mov		rsi, rax
	xor		rax, rax
	call	printf
	pop		rsi
	pop		rax
	pop		rdi

	jmp		.L207
.L107:
	mov		rdx, 2
	mov		[rbp-16], rdx

	push	rdi
	push	rax
	push	rsi

	mov		rdi, formInt
	mov		rax, [rbp-16]
	mov		rsi, rax
	xor		rax, rax
	call	printf
	pop		rsi
	pop		rax
	pop		rdi

.L207:
	add		rsp, 48
	pop		rbp
	mov		rax, 1
	xor		rbx, rbx
	int		0x80

	extern	printf
	extern	scanf

section .data
	formString db "%s", 10, 0
	formInt db "%d", 10, 0
	formInInt db "%d", 0
	bufInInt dq 0
