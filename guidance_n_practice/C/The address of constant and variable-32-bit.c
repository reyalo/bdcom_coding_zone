#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 128

int a = 1;						// .data
int b;							// .bss
const int c = 3;				// .rodata
static int d = 4;				// .data
char *str1 = "Hello, World!";	// .rodata

int main(void) 
{
	extern char __executable_start[];
	extern char edata[];
	extern char etext[];
	extern char end[];
	
	int aa = 1;						/* stack */
	int bb;							/* stack */
	const int cc = 3;				/* stack */
	static int dd = 4;				/* .data */
	char *str2 = "Hello, World!";	/* .rodata */
	char *str3 = malloc(BUF_LEN);	/* heap */

	printf("Start Address:%p\n", __executable_start);			// start address:0x8048000
	printf("Text Section:%p-%p\n", __executable_start, etext);	// Text Section:0x8048000-0x8048598
	printf("Data Section:%p-%p\n", etext, edata);				// Data Section:0x8048598-0x804a028
	printf("BSS  Section:%p-%p\n", edata, end);					// BSS  Section:0x804a028-0x804a030

	printf("&a=%p, &b=%p, &c=%p, &d=%p, str1=%p\n", &a, &b, &c, &d, str1);			// &a=0x804a018, &b=0x804a02c, &c=0x80485a4, &d=0x804a01c, str1=0x80485a8
	printf("&aa=%p, &bb=%p, &cc=%p, &dd=%p, str2=%p\n", &aa, &bb, &cc, &dd, str2);	// &aa=0xbffe80b8, &bb=0xbffe80b4, &cc=0xbffe80b0, &dd=0x804a024, str2=0x80485a8
	printf("&str3=%p\n", str3);		// &str3=0x9b42008
	
	{
		char buf[BUF_LEN] = {0};
		
		sprintf(buf, "cat /proc/%d/maps | grep -E 'heap|stack'", getpid());
		system(buf);
		/*
			09b42000-09b63000 rw-p 00000000 00:00 0          [heap]
			bfa47000-bfa68000 rw-p 00000000 00:00 0          [stack]
		*/
	}

	free(str3);
    return 0;
}

/*
readelf -S a.out 
There are 31 section headers, starting at offset 0x185c:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .interp           PROGBITS        08048154 000154 000013 00   A  0   0  1
  [ 2] .note.ABI-tag     NOTE            08048168 000168 000020 00   A  0   0  4
  [ 3] .note.gnu.build-i NOTE            08048188 000188 000024 00   A  0   0  4
  [ 4] .gnu.hash         GNU_HASH        080481ac 0001ac 000020 04   A  5   0  4
  [ 5] .dynsym           DYNSYM          080481cc 0001cc 000050 10   A  6   1  4
  [ 6] .dynstr           STRTAB          0804821c 00021c 00004c 00   A  0   0  1
  [ 7] .gnu.version      VERSYM          08048268 000268 00000a 02   A  5   0  2
  [ 8] .gnu.version_r    VERNEED         08048274 000274 000020 00   A  6   1  4
  [ 9] .rel.dyn          REL             08048294 000294 000008 08   A  5   0  4
  [10] .rel.plt          REL             0804829c 00029c 000010 08  AI  5  24  4
  [11] .init             PROGBITS        080482ac 0002ac 000023 00  AX  0   0  4
  [12] .plt              PROGBITS        080482d0 0002d0 000030 04  AX  0   0 16
  [13] .plt.got          PROGBITS        08048300 000300 000008 00  AX  0   0  8
  [14] .text             PROGBITS        08048310 000310 000272 00  AX  0   0 16
  [15] .fini             PROGBITS        08048584 000584 000014 00  AX  0   0  4
  [16] .rodata           PROGBITS        08048598 000598 0000c1 00   A  0   0  4	// str1 str2 c
  [17] .eh_frame_hdr     PROGBITS        0804865c 00065c 00002c 00   A  0   0  4
  [18] .eh_frame         PROGBITS        08048688 000688 0000b0 00   A  0   0  4
  [19] .init_array       INIT_ARRAY      08049f08 000f08 000004 04  WA  0   0  4
  [20] .fini_array       FINI_ARRAY      08049f0c 000f0c 000004 04  WA  0   0  4
  [21] .jcr              PROGBITS        08049f10 000f10 000004 00  WA  0   0  4
  [22] .dynamic          DYNAMIC         08049f14 000f14 0000e8 08  WA  6   0  4
  [23] .got              PROGBITS        08049ffc 000ffc 000004 04  WA  0   0  4
  [24] .got.plt          PROGBITS        0804a000 001000 000014 04  WA  0   0  4
  [25] .data             PROGBITS        0804a014 001014 000014 00  WA  0   0  4	// a d dd 
  [26] .bss              NOBITS          0804a028 001028 000008 00  WA  0   0  4	// b
  [27] .comment          PROGBITS        00000000 001028 00005a 01  MS  0   0  1
  [28] .symtab           SYMTAB          00000000 001084 0004c0 10     29  49  4
  [29] .strtab           STRTAB          00000000 001544 00020c 00      0   0  1
  [30] .shstrtab         STRTAB          00000000 001750 00010a 00      0   0  1
*/