#include <stdio.h>

int add(int a, int b, int c){
    int d = 0, e = 3;
    return a + b + c + d + e;
}

int main() {
    int a = 10;
    int b = 20;
    int sum = add(a, b, 30);

    return 0;
}

//assembly code of 2.c

/*

add:
    pushl	%ebp
    movl	%esp, %ebp
    subl	$16, %esp
    movl	$0, -8(%ebp)        ; d = 0 ; first variable is lower in memory
    movl	$3, -4(%ebp)        ; e = 3 ; second variable is higher in memory
    movl	8(%ebp), %edx       ; a     ; function arguments read from forward order
    movl	12(%ebp), %eax      ; b     ;
    addl	%eax, %edx          ; a + b
    movl	16(%ebp), %eax      ; c      ;
    addl	%eax, %edx          ; a + b + c
    movl	-8(%ebp), %eax      ; d      ;
    addl	%eax, %edx          ; a + b + c + d
    movl	-4(%ebp), %eax      ; e      ;
    addl	%edx, %eax          ; a + b + c + d + e
    leave                       ; movl %ebp, %esp ; reverse of 2nd line
                                ; popl %ebp ; reverse of 1st line
    ret                         ; pop value to eip(instruction pointer) to return to the caller
main:
    pushl	%ebp
    movl	%esp, %ebp
    subl	$16, %esp
    movl	$10, -12(%ebp)      ; a = 10 ; first variable is lower in memory
    movl	$20, -8(%ebp)       ; b = 20 ; second variable is higher in memory
                                ; function arguments pushed in reverse order
    pushl	$30                 ; push 30
    pushl	-8(%ebp)            ; push b
    pushl	-12(%ebp)           ; push a
    call	add
    addl	$12, %esp           ; release the stack for the function arguments
                                ; that means pop 3 values from the stack

    movl	%eax, -4(%ebp)      ; sum = add() ; store the return value in sum
    movl	$0, %eax            ; return 0  (return value is stored in eax)

    leave                       ; movl %ebp, %esp ; reverse of 2nd line
                                ; popl %ebp ; reverse of 1st line

    ret                         ; pop value to eip(instruction pointer) to return to the caller


*/