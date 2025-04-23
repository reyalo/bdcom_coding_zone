#include <taskLib.h>
#include<string.h>

typedef struct abc2{
    unsigned char a:2;
    unsigned char b:3;
    unsigned char c:4;

} test2;


typedef struct abc{
    unsigned short a:3;
    unsigned short b:2;
    unsigned short c:3;
    unsigned short d:3;
    unsigned short e:2;

} ABC;


extern unsigned long Print(char *format, ...);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
    

    // code for my selft testing
    int a = 513;
    char *p = (char *)&a;
    unsigned c0 = *p++;
    unsigned c1 = *p++;
    unsigned c2 = *p++;
    unsigned c3 = *p++;
    //printf("\n\nOutput in little Endian machine will be \"01 02 00 00\"\n\n");
    //printf("Output in Big Endian machine will be    \"00 00 02 01\"\n\n");
    //printf("The Byte Order in this machine low -> high = %02x %02x %02x %02x \n\n", c0, c1, c2, c3);
	
	Print("\n\nOutput in little Endian machine will be \"01 02 00 00\"\n\n");
    Print("Output in Big Endian machine will be    \"00 00 02 01\"\n\n");
    Print("The Byte Order in this machine low -> high = %02x %02x %02x %02x \n\n", c0, c1, c2, c3);


    // code of practice_6
    //unsigned char aa;
    //struct abc v;
    //v.a = 2;
    //v.b=3;
    //v.c=6;
    //memcpy(&aa, &v, sizeof(v));
    //printf("\n%x\n",aa);
	//Print("\n%x\n",aa);
	
	
	ABC test;
    unsigned short x = 0x2fc3;

    memcpy(&test, &x, sizeof(x));

    test.a = 13;
    test.b = 5;
    test.c = 8;
    test.d = 6;
    test.e = 2;

    memcpy(&x, &test, sizeof(x));

    //printf("%x\n", x);
    Print("%x\n", x);
	
	
	
	test2 sta;

    sta.a = 2;
    sta.b = 3;
    sta.c = 6;


    char *pp = (char *)&sta;
    p++;

    // printf("%x\n", *p);
    Print("%x\n", *pp);

    Print("%x \n", sta);
    //printf("%x \n", sta);
    
    

	return;
}
