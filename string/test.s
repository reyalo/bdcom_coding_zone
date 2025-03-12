
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$48, %esp
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax

	movb	$0, -41(%ebp)			; str

	movl	$544698184, -25(%ebp)	; str2
	movl	$543519329, -21(%ebp)
	movl	$1064660857, -17(%ebp)
	movb	$0, -13(%ebp)

	movl	$1819043144, -40(%ebp)	;str3
	movb	$111, -36(%ebp)			

	movl	$1819043144, -35(%ebp)	;str4
	movl	$111, -31(%ebp)
	movw	$0, -27(%ebp)
	
	movl	$0, %eax
	subl	$8, %esp
	
