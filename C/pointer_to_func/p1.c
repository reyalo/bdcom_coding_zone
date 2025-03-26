#include<stdio.h>

int divide(int num1, int num2) {
    return num1 / num2; 
}
int subtract(int num1, int num2) {
    return num1 - num2; 
}

int modulo(int num1, int num2) {
    return num1 % num2; 
}

typedef int (*fptrOperation)(int,int);


fptrOperation select(char opcode) {
    switch(opcode) {
    case '/': return divide;
    case '-': return subtract;
    case '%': return modulo;
    default : return NULL;
    }
}

int calculate(int num1, int num2, char opcode) {
    fptrOperation operation = select(opcode);
    return operation(num1, num2);
}

int main(void){

    
    int a = 7,b=3;
    printf("%d\n", calculate(a, b, '-'));
    printf("%d\n", calculate(a, b, '/'));
    printf("%d\n", calculate(a, b, '%'));
    return 0;

}