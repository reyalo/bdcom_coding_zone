#include <stdio.h>

void test_func(int v, int *p, int q[])
{
	v += 6;
	*p += 7;
	q[1] += 8;

	return;
}

int main(void) 
{
	int a = 1, b = 2;
	int c[3] = {3, 4, 5};

	test_func(a, &b, c);
    
    return 0;
}

/*
test_func:
        pushl   %ebp
        movl    %esp, %ebp
        addl    $6, 8(%ebp)			//	v = v + 6	access the value of a from main stack and add 6.
		
        movl    12(%ebp), %eax		//	access the address of b into the %eax (%eax = &b)
        movl    (%eax), %eax		//	now %eax contain value of variable b. (%eax = 2)
        leal    7(%eax), %edx		//	load the address of 7 + R[%eax] into %edx (%edx = 7 + 2)
        movl    12(%ebp), %eax		//	access the address of b into the %eax (%eax = &b)
        movl    %edx, (%eax)		//	move the value of %edx into the memory location of %eax (b = 9)
		
        movl    16(%ebp), %eax		//	load the first address of array c into the %eax (%eax = &c[0])
        addl    $4, %eax			//	add 4 bytes to move the next location of the array (%eax = &c[1])
        movl    16(%ebp), %edx		//	edx contain the address of first element (%edx = &c[0])
        addl    $4, %edx			//	move 4 bytes to goto the next location (%edx = &c[1])
        movl    (%edx), %edx		// 	load the value of second element into the %edx (%edx = 4)
        addl    $8, %edx			//	add 8 with the existing value of %edx (%edx = 4 + 8)
        movl    %edx, (%eax)		// 	move the result to the c[1] location.  (c[1] = 12)
        nop
        popl    %ebp
        ret
main:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $44, %esp
        movl    $1, -4(%ebp)		// a = 1
        movl    $2, -8(%ebp)		// b = 2
        movl    $3, -20(%ebp)		// c[0] = 3
        movl    $4, -16(%ebp)		// c[1] = 4
        movl    $5, -12(%ebp)		// c[2] = 5
		
        leal    -20(%ebp), %eax		// Load the address of the first element of array c into the eax, %eax = &c[0].
        movl    %eax, 8(%esp)		// put the address of c into the 3rd top of current stack
        leal    -8(%ebp), %eax		// load the address of b into eax, %eax = &b
        movl    %eax, 4(%esp)		// put the address of b into the second top of current stack
        movl    -4(%ebp), %eax		// put the value of a into %eax, %eax = a
        movl    %eax, (%esp)		// put the value of a into the top of current stack
        call    test_func
        movl    $0, %eax
        leave
        ret
*/