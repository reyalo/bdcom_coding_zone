#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 128

int a = 1;						// 0x55af645c7010 - 0x55af645c3000 = 0x4010
int b;							// 0x55af645c702c - 0x55af645c3000 = 0x4020
const int c = 3;				// 0x55af645c5008 - 0x55af645c3000 = 0x2008
static int d = 4;				// 0x55af645c7014 - 0x55af645c3000 = 0x4014
char *str1 = "Hello, World!";	// 0x55af645c500c - 0x55af645c3000 = 0x200C

int main(void) 
{
	extern char __executable_start[];
	extern char edata[];
	extern char etext[];
	extern char end[];
	
	int aa = 1;						/* 0x7ffd9245d564 is locate in 7ffd9243f000-7ffd92460000 */
	int bb;							/* 0x7ffd9245d568 is locate in 7ffd9243f000-7ffd92460000 */
	const int cc = 3;				/* 0x7ffd9245d56c is locate in 7ffd9243f000-7ffd92460000 */
	static int dd = 4;				/* 0x55af645c7018 - 0x55af645c3000 = 0x4018 */
	char *str2 = "Hello, World!";	/* 0x55af645c500c - 0x55af645c3000 = 0x200C */
	char *str3 = malloc(BUF_LEN);	/* 0x55af68e742a0 is locate in 55af68e74000-55af68e95000 */

	printf("Start Address:%p\n", __executable_start);			// Start Address:0x55af645c3000
	printf("Text Section:%p-%p\n", __executable_start, etext);	// Text Section:0x55af645c3000-0x55af645c44a5
	printf("Data Section:%p-%p\n", etext, edata);				// Data Section:0x55af645c44a5-0x55af645c7028
	printf("BSS  Section:%p-%p\n", edata, end);					// BSS  Section:0x55af645c7028-0x55af645c7030

	printf("&a=%p, &b=%p, &c=%p, &d=%p, str1=%p\n", &a, &b, &c, &d, str1);			// &a=0x55af645c7010, &b=0x55af645c702c, &c=0x55af645c5008, &d=0x55af645c7014, str1=0x55af645c500c
	printf("&aa=%p, &bb=%p, &cc=%p, &dd=%p, str2=%p\n", &aa, &bb, &cc, &dd, str2);	// &aa=0x7ffd9245d564, &bb=0x7ffd9245d568, &cc=0x7ffd9245d56c, &dd=0x55af645c7018, str2=0x55af645c500c
	printf("&str3=%p\n", str3);		// &str3=0x55af68e742a0
	
	{
		char buf[BUF_LEN] = {0};
		
		sprintf(buf, "cat /proc/%d/maps | grep -E 'heap|stack'", getpid());
		system(buf);
		/*
			55af68e74000-55af68e95000 rw-p 00000000 00:00 0                          [heap]
			7ffd9243f000-7ffd92460000 rw-p 00000000 00:00 0                          [stack]
		*/
	}

    return 0;
}

/*
readelf -S a.out 

There are 31 section headers, starting at offset 0x3b78:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000000318  00000318
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .note.gnu.propert NOTE             0000000000000338  00000338
       0000000000000020  0000000000000000   A       0     0     8
  [ 3] .note.gnu.build-i NOTE             0000000000000358  00000358
       0000000000000024  0000000000000000   A       0     0     4
  [ 4] .note.ABI-tag     NOTE             000000000000037c  0000037c
       0000000000000020  0000000000000000   A       0     0     4
  [ 5] .gnu.hash         GNU_HASH         00000000000003a0  000003a0
       0000000000000024  0000000000000000   A       6     0     8
  [ 6] .dynsym           DYNSYM           00000000000003c8  000003c8
       0000000000000120  0000000000000018   A       7     1     8
  [ 7] .dynstr           STRTAB           00000000000004e8  000004e8
       00000000000000b5  0000000000000000   A       0     0     1
  [ 8] .gnu.version      VERSYM           000000000000059e  0000059e
       0000000000000018  0000000000000002   A       6     0     2
  [ 9] .gnu.version_r    VERNEED          00000000000005b8  000005b8
       0000000000000030  0000000000000000   A       7     1     8
  [10] .rela.dyn         RELA             00000000000005e8  000005e8
       00000000000000d8  0000000000000018   A       6     0     8
  [11] .rela.plt         RELA             00000000000006c0  000006c0
       0000000000000090  0000000000000018  AI       6    24     8
  [12] .init             PROGBITS         0000000000001000  00001000
       000000000000001b  0000000000000000  AX       0     0     4
  [13] .plt              PROGBITS         0000000000001020  00001020
       0000000000000070  0000000000000010  AX       0     0     16
  [14] .plt.got          PROGBITS         0000000000001090  00001090
       0000000000000010  0000000000000010  AX       0     0     16
  [15] .plt.sec          PROGBITS         00000000000010a0  000010a0
       0000000000000060  0000000000000010  AX       0     0     16
  [16] .text             PROGBITS         0000000000001100  00001100
       0000000000000395  0000000000000000  AX       0     0     16
  [17] .fini             PROGBITS         0000000000001498  00001498
       000000000000000d  0000000000000000  AX       0     0     4
  [18] .rodata           PROGBITS         0000000000002000  00002000		// str1 str2 c
       00000000000000f1  0000000000000000   A       0     0     8
  [19] .eh_frame_hdr     PROGBITS         00000000000020f4  000020f4
       0000000000000044  0000000000000000   A       0     0     4
  [20] .eh_frame         PROGBITS         0000000000002138  00002138
       0000000000000108  0000000000000000   A       0     0     8
  [21] .init_array       INIT_ARRAY       0000000000003d90  00002d90
       0000000000000008  0000000000000008  WA       0     0     8
  [22] .fini_array       FINI_ARRAY       0000000000003d98  00002d98
       0000000000000008  0000000000000008  WA       0     0     8
  [23] .dynamic          DYNAMIC          0000000000003da0  00002da0
       00000000000001f0  0000000000000010  WA       7     0     8
  [24] .got              PROGBITS         0000000000003f90  00002f90
       0000000000000070  0000000000000008  WA       0     0     8
  [25] .data             PROGBITS         0000000000004000  00003000		// a d dd
       0000000000000028  0000000000000000  WA       0     0     8
  [26] .bss              NOBITS           0000000000004028  00003028		// b
       0000000000000008  0000000000000000  WA       0     0     4
  [27] .comment          PROGBITS         0000000000000000  00003028
       000000000000002b  0000000000000001  MS       0     0     1
  [28] .symtab           SYMTAB           0000000000000000  00003058
       0000000000000780  0000000000000018          29    48     8
  [29] .strtab           STRTAB           0000000000000000  000037d8
       0000000000000285  0000000000000000           0     0     1
  [30] .shstrtab         STRTAB           0000000000000000  00003a5d
       000000000000011a  0000000000000000           0     0     1
*/