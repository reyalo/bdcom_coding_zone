#include <stdio.h>

int fun1(int m, int n);
int fun2(int m, int n);

int fun2(int m, int n)
{
    int aaa = 111;
    int bbb = 222;

    return m + aaa + n + bbb;
}

int fun1(int m, int n)
{
    int aa = 11;
    int bb = 22;
    
    return fun2(aa + m, bb + n);
}

int main(void) 
{
    int a = 1;
    int b = 2;
    
    fun1(a , b);
    return 0;
}

/*
fun2:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        movl    $111, -4(%ebp)		// aaa = 111, alloc space for local variable aaa
        movl    $222, -8(%ebp)		// bbb = 222, alloc space for local variable bbb
        movl    8(%ebp), %edx		// Copy the value of actual parameter aa + m from fun1 to register, prepare for the calculation
        movl    -4(%ebp), %eax		// Copy the value of local variable aaa to register, prepare for the calculation
        addl    %eax, %edx			// m(formal parameter m stands for actual parameter aa + m) + aaa
        movl    12(%ebp), %eax		// Copy the value of actual parameter bb + n from fun1 to register, prepare for the calculation
        addl    %eax, %edx			// n(formal parameter n stands for actual parameter bb + n) + the result of m + aaa
        movl    -8(%ebp), %eax		// Copy the value of local variable bbb to register, prepare for the calculation
        addl    %edx, %eax			// bbb + the result of m + aaa + n
        leave						// move esp to ebp's location, pop ebp
        ret							// return back to the instruction next "call fun2"
fun1:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        movl    $11, -4(%ebp)		// aa = 11, alloc space for local variable aa
        movl    $22, -8(%ebp)		// bb = 22, alloc space for local variable bb
        movl    -8(%ebp), %edx		// Copy the value of local variable bb to register, prepare for the calculation
        movl    12(%ebp), %eax		// Copy the value of actual parameter b from main to register, prepare for the calculation
        addl    %eax, %edx			// bb + n(formal parameter n stands for actual parameter b)
        movl    -4(%ebp), %ecx		// Copy the value of local variable aa to register, prepare for the calculation
        movl    8(%ebp), %eax		// Copy the value of actual parameter a from main to register, prepare for the calculation
        addl    %ecx, %eax			// aa + m(formal parameter m stands for actual parameter a)
        pushl   %edx				// The result of bb + n
        pushl   %eax				// The result of aa + m
        call    fun2				// fun2(aa + m, bb + n)
        addl    $8, %esp			// move esp to the location where return address on, prepare for pop
        leave						// move esp to ebp's location, pop ebp
        ret							// return back to the instruction next "call fun1"
main:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        movl    $1, -4(%ebp)		// a = 1, alloc space for local variable a
        movl    $2, -8(%ebp)		// b = 2, alloc space for local variable b
        pushl   -8(%ebp)			// actual parameter b of fun1, a copy from local variable b
        pushl   -4(%ebp)			// actual parameter a of fun1, a copy from local variable a
        call    fun1				// fun1(a , b)
        addl    $8, %esp			// move esp to the location where return address on, prepare for pop
        movl    $0, %eax			// eax stored the finally result, reset it now
        leave
        ret
*/