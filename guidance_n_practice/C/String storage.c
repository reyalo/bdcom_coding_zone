/* Program run on 32 bits platform */
#include <stdio.h>

// str11 is a pointer that takes 4 bytes on data section and stores an address to rodata section. Which stores the string
char *str11 = "abc de";
	
// str22 is an array that takes sizeof(str22) bytes on data section. 
char str22[] = "abc de";
	
// str33 is an array that takes sizeof(str33) bytes on data section. 
char str33[] = {'a', 'b', 'c', ' ', 'd', 'e', '\0'};
	
// str44 is a pointer that takes 4 bytes on bss
char *str44 = NULL;

// same with str22
static char str55[] = "abc de";

int main(void) 
{
	// str1 is a pointer that takes 4 bytes on stack and stores an address to rodata section. Which stores the string
	char *str1 = "abc de";
	
	// str2 is an array that takes sizeof(str2) bytes on stack. "abc de" is pushed into stack like some numbers
	char str2[] = "abc de";
	
	// str3 is an array that takes sizeof(str3) bytes on stack. The characters are pushed into stack as ASCII number
	char str3[] = {'a', 'b', 'c', ' ', 'd', 'e', '\0'};
	
	// The string is stored on rodata section.
	// str4 is a pointer that takes 4 bytes on stack and stores an address to heap used to store the string from rodata section.
	char *str4 = malloc(10);
	if (str4)
	{
		memcpy(str4, "abc de", sizeof("abc de"));
		free(str4);
		str4 = NULL;
	}
	
	// The string is stored on rodata section.
	// str44 stores an address to heap used to store the string from rodata section.
	str44 = malloc(10);
	if (str44)
	{
		memcpy(str44, "abc de", sizeof("abc de"));
		free(str44);
		str44 = NULL;
	}
	
	// same with str22
	static char str5[] = "abc de";
    return 0;
}