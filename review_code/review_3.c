#include<stdio.h>






int main()
{
    int a[4] = {1, 2, 3, 4};

    printf("%d %p\n", sizeof(a + 0), a + 0);        // 4,  0x1000
    printf("%d %p\n", sizeof(*a), *a);              // 4,  0x0001
    printf("%d %p\n", sizeof(*&a), *&a);            // 16, 0x1000
    printf("%d %p\n", sizeof(*&a + 1), *&a + 1);    // 4,  0x1004
    printf("%d %p\n", sizeof(&*a), &*a);            // 4,  0x1000  // first deref to 1 then take address of 1 that means (int *) not whole array
    printf("%d %p\n", sizeof(&*a + 1), &*a + 1);    // 4,  0x1004  // final result as (int *), so increment will 4 bytes

    return 0;
}

/*
1.
    int arr[5] = {};
    int *p = &arr[1], *q = &arr[3];

    printf("%lu\n", q - p);


2.
    int array_2d[2][3] = {1, 2, 3, 4, 5, 6};
    int *p, *p2d;

    p2d = (int *)array_2d;

    for (p2d = (int *)array_2d; p2d < (int *)(array_2d + 2); p2d += 3) {
        for (p = p2d; p < p2d + 3; p++) {
            printf("%d ", *p);
        }
        putchar('\n');
    }



3.

void func(void){

    printf("Hello World\n");
    return;

}


    int a = (int)func;

    ((void (*)(void))a )();     // typedef void (*fptr_func)(void);
                                ;// (fptr_func a)();




4.

;// how to change the value of the address 0x100000 to 0xaa55

    int *ptr;

    ptr = (int *)0x100000;

    *ptr = 0xaa55;

    printf("%x\n", *ptr);



5.

    int a[5] = {1, 2, 3, 4, 5};

    int *ptr = (int *)(&a + 1);
    printf("%d %d\n", *(a + 1), *(ptr - 1));


6.

expression          subscript        point to        +1
---------------------------------------------------------
2d_arr              2d_arr[][]      size

2d_arr[1]

&2d_arr[1]

&2d_arr


7.

struct Test{

    int num;
    char *name;
    short sDate;
    char cha[2];
    short sBa[4];

} *p, d;

;// how many away access the name;
printf("%s\n", d.name);
p = &d;
printf("%s\n", p->name);








8.

struct Test{

    int num;        // 4 byte
    char *name;     // 4 byte
    short sDate;    // 2 byte
    char cha[2];    // 2 byte
    short sBa[4];   // 8 byte

} *p = NULL, d;


    printf("%p\n", p + 1);                   // 0x00014 //0x00020(for 64)
    printf("%p\n", (unsigned long)p + 1);    // 0x00001
    printf("%p\n", (unsigned int *)p + 1);   // 0x00001



9.

    int a[4] = {1, 2, 3, 4};
    int *ptr1 = (int *)(&a + 1);
    int *ptr2 = (int *)((int)a + 1);

    printf("%x %x \n", ptr1[-1], *ptr2); // 00 00 00 04, 02 00 00 00



10.


    int a[4] = {1, 2, 3, 4};
    int *ptr1 = (int *)(&a + 1);
    int *ptr2 = (int *)((int)a + 1);

    printf("%x %x \n", *((int *)((long)a + 3)), *((int *)((char *)a + 3)));
    printf("%x %x \n", *((int *)((long)a + 3)), *((int *)((char *)a + 3)));
    printf("%x %x \n", *((long *)((long)a + 3)), *((long *)((char *)a + 3))); // long = 4(32), long = 8(64)

    ;// output(32): 00 00 02 00, 00 00 02 00



11.


    int a[4] = {1, 2, 3, 4};

    printf("%d %p\n", sizeof(&a[0]), &a[0]);    // 4, 1000
    printf("%d %p\n", sizeof(a), a);            // 16, 1000
    printf("%d %p\n", sizeof(&a), &a);          // 4,  1000


12.


    int a[4] = {1, 2, 3, 4};

    printf("%d %p\n", sizeof(a + 0), a + 0);        // 4,  0x1000
    printf("%d %p\n", sizeof(*a), *a);              // 4,  0x0001
    printf("%d %p\n", sizeof(*&a), *&a);            // 16, 0x1000
    printf("%d %p\n", sizeof(*&a + 1), *&a + 1);    // 4,  0x1004



13.

    int a[4] = {1, 2, 3, 4};

    printf("%d %p\n", sizeof(a + 0), a + 0);        // 4,  0x1000
    printf("%d %p\n", sizeof(*a), *a);              // 4,  0x0001
    printf("%d %p\n", sizeof(*&a), *&a);            // 16, 0x1000
    printf("%d %p\n", sizeof(*&a + 1), *&a + 1);    // 4,  0x1004
    printf("%d %p\n", sizeof(&*a), &*a);            // 4,  0x1000  // first deref to 1 then take address of 1 that means (int *) not whole array
    printf("%d %p\n", sizeof(&*a + 1), &*a + 1);    // 4,  0x1004  // final result as (int *), so increment will 4 bytes


*/