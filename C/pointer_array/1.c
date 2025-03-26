#include<stdio.h>

int add(int a, int b, int c){
    int d = 0, e = 0;
    return a + b + c + d + 3;
}

int main(){

    // int a = 10;
    // int b = 20;

    int sum = add(10, 20, 30);

    return 0;
}

/*

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



*/