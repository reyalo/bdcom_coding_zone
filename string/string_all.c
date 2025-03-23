#include<stdio.h>
#include<string.h>

void print_char(const char *str, size_t sz){
    for(int i = 0; i < sz; i++){
        printf("%u ", str[i]);
    }
    printf("\n");
}

void print_charc(const char *str, size_t sz){
    for(int i = 0; i < sz; i++){
        printf("%c ", str[i]);
    }
    printf("\n");
}



int main(){


    // power of sizeof() operator : it returns original space allocated for the array, it executes at compile time

    char str[] = "";                                // size = 1
    char str2[] = "How are you?";                   // size = 13

    // Explicitly initializing the array
    char str3[] = {'H', 'e', 'l', 'l', 'o'};        // size = 5, No null terminator, so undefined behavior

    // Explicitly initializing the array with size
    char str4[10] = {'M', 'e', 'l', 'l', 'o'};      // size = 10, Partial initialization, so rest of the elements are 0

    printf("String length: %lu\n", strlen(str));    // strlen() will count until it finds a null terminator
    printf("String length: %lu\n", strlen(str2));   
    printf("String length: %lu\n", strlen(str3));   
    printf("String length: %lu\n", strlen(str4));   



    // Printing the ASCII values of the characters
    print_char(str, sizeof(str));
    print_char(str2, sizeof(str2));
    print_char(str3, sizeof(str3));
    print_char(str4, sizeof(str4));

    strcpy(str3, "123");
    print_char(str3, sizeof(str3));

    //_________________________________________strcpy(Each Analysis Independent)________________________________________________


    //? after copying str2 to str, sizeof() str no change
    // strcpy(str, str2);                              // it illegal to copy a string to an array that is not big enough to hold it
    // printf("str: %s\n", str);                        // output : How are you? (but it occopied other's memory location)
    // printf("Size of str: %lu\n", sizeof(str));       // output : 1


    



    // printf("Str3 : %s\n", str3);                // Expected:Hello // output : HelloMello // str3 is not null terminated, so undefined behavior

    // printf("\n\n");
    // printf("str: %s\n", str);
    // strcpy(str, str3);                          // str3 is not null terminated, so undefined behavior
    // printf("str: %s\n", str);                    //Expected:Hello // output : HelloMello // str3 is not null terminated, so it coppies until it finds a null terminator










    //_________________________________________strcat(Each Analysis Independent)________________________________________________





    
}

// Assembly code
/*

gcc -m32 -S 1.c -o 1.s


main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$48, %esp
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax

	movb	$0, -41(%ebp)			; str

	movl	$544698184, -25(%ebp)	; str2
	movl	$543519329, -21(%ebp)
	movl	$1064660857, -17(%ebp)
	movb	$0, -13(%ebp)

	movl	$1819043144, -40(%ebp)	;str3
	movb	$111, -36(%ebp)			

	movl	$1819043144, -35(%ebp)	;str4
	movl	$111, -31(%ebp)
	movw	$0, -27(%ebp)
	
	movl	$0, %eax
	subl	$8, %esp
	


*/