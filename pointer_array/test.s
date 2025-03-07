	.file	"1.c"
	.text
	.globl	add
	.type	add, @function
add:
.LFB0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -8(%ebp)
	movl	$0, -4(%ebp)
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	addl	%eax, %edx
	movl	16(%ebp), %eax
	addl	%eax, %edx
	movl	-8(%ebp), %eax
	addl	%edx, %eax
	addl	$3, %eax
	leave
	ret


.LFE0:
	.size	add, .-add
	.globl	main
	.type	main, @function
main:
.LFB1:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	$30
	pushl	$20
	pushl	$10
	call	add
	addl	$12, %esp
	movl	%eax, -4(%ebp)
	movl	$0, %eax
	leave
	ret

