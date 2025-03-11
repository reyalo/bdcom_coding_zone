	.file	"p9.c"
	.text
	.section	.rodata
.LC0:
	.string	"Hello World!"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	%gs:20, %edx
	movl	%edx, 28(%esp)
	xorl	%edx, %edx
	leal	.LC0@GOTOFF(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$1819043144, 15(%esp)
	movl	$1867980911, 19(%esp)
	movl	$6581362, 23(%esp)
	movb	$0, 27(%esp)
	movl	8(%esp), %eax
	addl	$2, %eax
	movb	$11, (%eax)
	movb	$22, 17(%esp)
	movl	$0, %eax
	movl	28(%esp), %edx
	subl	%gs:20, %edx
	je	.L3
	call	__stack_chk_fail_local
.L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB1:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE1:
	.hidden	__stack_chk_fail_local
	.ident	"GCC: (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0"
	.section	.note.GNU-stack,"",@progbits
