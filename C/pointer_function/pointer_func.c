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

        printf("%d %d %d\n", a, b, c[1]);
    
    return 0;
}

/*
test_func:
        pushl   %ebp
        movl    %esp, %ebp
        addl    $6, 8(%ebp)
		
        movl    12(%ebp), %eax
        movl    (%eax), %eax
        leal    7(%eax), %edx
        movl    12(%ebp), %eax
        movl    %edx, (%eax)
		
        movl    16(%ebp), %eax
        addl    $4, %eax
        movl    16(%ebp), %edx
        addl    $4, %edx
        movl    (%edx), %edx
        addl    $8, %edx
        movl    %edx, (%eax)
        nop
        popl    %ebp
        ret
main:
        pushl   %ebp
        movl    %esp, %ebp
        subl    $44, %esp
        movl    $1, -4(%ebp)
        movl    $2, -8(%ebp)
        movl    $3, -20(%ebp)
        movl    $4, -16(%ebp)
        movl    $5, -12(%ebp)
		
        leal    -20(%ebp), %eax
        movl    %eax, 8(%esp)
        leal    -8(%ebp), %eax
        movl    %eax, 4(%esp)
        movl    -4(%ebp), %eax
        movl    %eax, (%esp)
        call    test_func
        movl    $0, %eax
        leave
        ret
*/