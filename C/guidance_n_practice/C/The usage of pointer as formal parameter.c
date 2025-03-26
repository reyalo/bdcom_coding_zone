#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARR_NUM 3
#define ARR_NUM2 2

void funca1(int a[], int len)
{
	int i;
	
	for (i = 0; i < len; i++)
		printf("%d ", a[i]);
	
	putchar('\n');
	return;
}

void funca2(int a[ARR_NUM])
{
	int i;
	
	for (i = 0; i < ARR_NUM; i++)
		printf("%d ", a[i]);
	
	putchar('\n');
	return;
}

void funca3(int *a, int len)
{
	int i;
	
	for (i = 0; i < len; i++)
		printf("%d ", a[i]);
	
	putchar('\n');
	return;
}

void funcb1(int *b[], int len)
{
	int i;
	
	for (i = 0; i < len; i++)
		printf("%d %d ", b[i][0], b[i][1]);
	
	putchar('\n');
	return;
}

void funcb2(int *b[ARR_NUM])
{
	int i;
	
	for (i = 0; i < ARR_NUM; i++)
		printf("%d %d ", b[i][0], b[i][1]);
	
	putchar('\n');
	return;
}

void funcb3(int **b, int len)
{
	int i;
	
	for (i = 0; i < len; i++)
		printf("%d %d ", b[i][0], b[i][1]);
	
	putchar('\n');
	return;
}

void funcc1(int (*c)[ARR_NUM], int end_mem_addr)
{
	int *p;
	
	for(; (int)c < end_mem_addr; c++)
		for(p = (int*)c; (int)p < (int)(c + 1); p++)
			printf("%d ", *p);
	
	putchar('\n');
	return;
}

void funcc2(int **c, int end_mem_addr)
{
	for(; (int)c < end_mem_addr; c++)
		printf("%d ", *c);
	
	putchar('\n');
	return;
}

void funcd1(int d[ARR_NUM2][ARR_NUM])
{
	int i, j;
	
	for (i = 0; i < ARR_NUM2; i++)
		for (j = 0; j < ARR_NUM; j++)
			printf("%d ", d[i][j]);
			
	putchar('\n');
	return;
}

void funcd2(int d[][ARR_NUM], int row)
{
	int i, j;
	
	for (i = 0; i < row; i++)
		for (j = 0; j < ARR_NUM; j++)
			printf("%d ", d[i][j]);
			
	putchar('\n');
	return;
}

void funcd3(int *d[ARR_NUM], int row)			// Advanced usage, not recommended for now
{
	int i;
	
	for (i = 0; i < row * ARR_NUM; i++)
		printf("%d ", d[i]);
			
	putchar('\n');
	return;
}

void funcd4(int (*d)[ARR_NUM], int end_mem_addr)
{
	int *p;
	
	for(; (int)d < end_mem_addr; d++)
		for(p = (int*)d; (int)p < (int)(d + 1); p++)
			printf("%d ", *p);
	
	putchar('\n');
	return;
}

void funcd5(int **d, int end_mem_addr)
{
	for(; (int)d < end_mem_addr; d++)
		printf("%d ", *d);
	
	putchar('\n');
	return;
}

void funce1(int **e, int len)
{
	int i;
	
	for(i = 0; i < len; i++)
		printf("%d %d ", e[i][0], e[i][1]);
	
	putchar('\n');
	return;
}

void funce2(int **e, int end_mem_addr)
{
	for(; (int)e < end_mem_addr; e++)
		printf("%d ", *e);
	
	putchar('\n');
	return;
}

int main(int argc, char *argv[], char *envp[])
{
    int a[ARR_NUM] = {1, 2, 3};
    int *b[ARR_NUM] = {(int []){1, 2}, (int []){3, 4},(int []){5, 6}};
    int (*c)[ARR_NUM];
    int d[ARR_NUM2][ARR_NUM] = {1, 2, 3, 4, 5, 6};
    int **e;
	
	c = d;
    
    fputs("one-dimensional array\n", stdout);
    funca1(a, ARR_NUM);
    funca2(a);
    funca3(a, ARR_NUM);
	putchar('\n');
 
	fputs("array of pointer\n", stdout);
    funcb1(b, ARR_NUM);
	funcb2(b);
	funcb3(b, ARR_NUM);
	putchar('\n');
	
	fputs("pointer to array\n", stdout);
	funcc1(c, (int)(&d + 1));
	funcc2((int **)c, (int)(&d + 1));
	putchar('\n');
	
	fputs("multi-dimensional array\n", stdout);
	funcd1(d);
	funcd2(d, ARR_NUM2);
	funcd3(d, ARR_NUM2);
	funcd4(d, (int)(&d + 1));
	funcd5((int **)d, (int)(&d + 1));
	putchar('\n');
    
	fputs("pointer to pointer\n", stdout);
	e = b;
	funce1(e, ARR_NUM);
	e = (int**)d;
	funce2(e, (int)(&d + 1));
	putchar('\n');
	
    return 0;
}