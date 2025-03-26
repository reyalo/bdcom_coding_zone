#include<stdio.h>






int main()
{

    
    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = (long *)(array + 1);
    p2 = (long *)(*(array + 1));

    printf("%lx, %lx\n", *( (long *)((char (*)[6])p1 - 1) ) + 1, *( (long *)((char (*)[5])p2 + 1)  + 1)  );

    //*32 output : 00 03 00 01, 07 00 00 00
    //*64 output : 00 04 00 00 00 00 00 01, 00 00 00 00 06 00 00 00



    

    return 0;
}

/*




    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = (long *)(array + 1);
    p2 = (long *)(*(array + 1));

    printf("%lx, %lx\n", *( (long *)((char (*)[6])p1 - 1) ) + 1, *( (long *)((char (*)[5])p2 + 1)  + 1)  );

    //*32 output : 00 03 00 01, 07 00 00 00
    //*64 output : 00 04 00 00 00 00 00 01, 00 00 00 00 06 00 00 00

 



    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    ;// p1 = &array[2];
    ;// p2 = &array[3];
    p1 = array[2];
    p2 = array[3];

    printf("%lx, %lx\n", *( (long *)((char *)p1 + 1) ) + 1, *( (long *)((long)p2 - 1) + 1 ) ); // try with different sign

    //*32 output: 08 00 00 01, 00 00 0b 00
    //*64 output: 08 00 00 00 00 00 00 01, 00 00 00 00 00 00 0b 00






    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = &array[2][4];
    p2 = &array[1][8];

    printf("%lx, %lx\n", *( (long *)((long)p1 + 1) ), *( (int *)((long)p2 - 1) ) );
    //*32 output : 0c 00 00 00, 00 00 0c 00
    //*64 output : 0c 00 00 00 00 00 00 00, 00 00 0c 00







;//9.
    char arr[] = "abcde";
    char *p = "abcde";

    printf("%d %d\n", sizeof(arr), sizeof(p));              //* 6 4
    printf("%d %d\n", sizeof(arr + 0), sizeof(p + 0));      //* 4 4
    printf("%d %d\n", sizeof(*arr), sizeof(*p));            //* 1 1
    printf("%d %d\n", strlen(&arr), strlen(&p));            //* 5 undef
    printf("%d %d\n", strlen(&arr + 1), strlen(&p + 1));    //* undef undef





    char *array1[3] = {"1234", "34567", "56789"};

    char **p1 = array1;

    char array2[2][5] = {"12345", "abcd"};
    char (*p2)[5] = array2;

    printf("%d, %s, %s, %c, %c\n", sizeof(array1), *p1 + 2, *(p1 + 2), **p1 + 2, **(p1 + 2) );

    //* output : 12, 34, 56789, 3, 5




    char *array1[3] = {"1234", "34567", "56789"};

    char **p1 = array1;

    char array2[2][5] = {"12345", "abcd"};
    char (*p2)[5] = array2;

    p1 = (char **)((char *)(p2 + 1) + 3);


    printf("%s, %s, %s, %c\n", *p2, p2 + 1, *p2 + 1, *p1 );     //* 12345abcd, abcd, 2345abcd, d




    int a[3][4] = {0};

    printf("%p, %p\n", a[0][0], a[0][0] + 1);       //* 0x0 0x1
    printf("%p, %p\n", &a[0][0], &a[0][0] + 1);     //* 0x1000 0x1004
    printf("%p, %p\n", a[0], a[0] + 1);             //* 0x1000 0x1004





    int a[3][4] = {0};

    printf("%p, %p\n", *(a + 1), a + 1);                                //* 0x1010 0x1010
    printf("%d, %d\n", sizeof( *(a + 1) ), sizeof( *(a[0] + 1) ) );     //* 16   4
    printf("%d, %d\n", sizeof( *(&a[0] + 1) ), sizeof( *(&a + 1) ) );   //* 16   48




    int a[3][4] = {0};

    printf("%p, %p\n", &a[0], &a[0] + 1);       //* 0x1000 0x1010
    printf("%p, %p\n", a, a + 1);               //* 0x1000 0x1010
    printf("%p, %p\n", &a, &a + 1);             //* 0x1000 0x1030


    char arr[] = "abcde";
    char *p = "abcde";

    printf("%d %d\n", sizeof(arr), sizeof(p));
    printf("%d %d\n", sizeof(arr + 0), sizeof(p + 0));
    printf("%d %d\n", sizeof(*arr), sizeof(*p));
    printf("%d %d\n", strlen(&arr), strlen(&p));
    printf("%d %d\n", strlen(&arr + 1), strlen(&p + 1));












    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = &array[2];
    p2 = &array[3];
    ;// p1 = array[2];
    ;// p2 = array[3];

    printf("%lx, %lx\n", *( (long *)((char *)p1 + 1) ) + 1, *( (long *)((long)p2 - 1) + 1 ) ); // try with different sign





    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = &array[2][4];
    p2 = &array[1][8];

    printf("%lx, %lx\n", *( (long *)((long)p1 + 1) ), *( (int *)((long)p2 - 1) ) );






    long array[4][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    long *p1 = NULL, *p2 = NULL;

    p1 = (long *)(array + 1);
    p2 = (long *)(*(array + 1));

    printf("%lx, %lx\n", *( (long *)((char (*)[6])p1 - 1) ) + 1, *( (long *)((char (*)[5])p2 + 1)  + 1)  );







    char *array1[3] = {"1234", "34567", "56789"};

    char **p1 = array1;

    char array2[2][5] = {"12345", "abcd"};
    char (*p2)[5] = array2;

    printf("%d, %s, %s, %c, %c\n", sizeof(array1), *p1 + 2, *(p1 + 2), **p1 + 2, **(p1 + 2) );







    char *array1[3] = {"1234", "34567", "56789"};

    char **p1 = array1;

    char array2[2][5] = {"12345", "abcd"};
    char (*p2)[5] = array2;

    p1 = (char **)((char *)(p2 + 1) + 3);


    printf("%s, %s, %s, %c\n", *p2, p2 + 1, *p2 + 1, *p1 );





    int a[3][4] = {0};

    printf("%p, %p\n", a[0][0], a[0][0] + 1);
    printf("%p, %p\n", &a[0][0], &a[0][0] + 1);
    printf("%p, %p\n", a[0], a[0] + 1);





    int a[3][4] = {0};

    printf("%p, %p\n", *(a + 1), a + 1);
    printf("%d, %d\n", sizeof( *(a + 1) ), sizeof( *(a[0] + 1) ) );
    printf("%d, %d\n", sizeof( *(&a[0] + 1) ), sizeof( *(&a + 1) ) );




    int a[3][4] = {0};

    printf("%p, %p\n", &a[0], &a[0] + 1);
    printf("%p, %p\n", a, a + 1);
    printf("%p, %p\n", &a, &a + 1);



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