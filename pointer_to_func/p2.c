#include <stdio.h>

// Define operation types
typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} Operation;

// Function prototypes
double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }
double div(double a, double b) { return b != 0 ? a / b : 0; }

// Define function pointer type
typedef double (*operation)(double, double);

// Create jump table
operation operations[] = {
    [ADD] = add,
    [SUB] = sub,
    [MUL] = mul,
    [DIV] = div
};



int main() {
    double op1, op2, result;
    Operation oper;

    // Get input from user
    printf("Enter first number: ");
    scanf("%lf", &op1);
    printf("Enter second number: ");
    scanf("%lf", &op2);
    printf("Enter operation (0:ADD, 1:SUB, 2:MUL, 3:DIV): ");
    scanf("%d", (int*)&oper);

    // Validate operation
    if (oper >= 0 && oper <= 3) {
        // Use jump table to perform operation
        result = operations[oper](op1, op2);
        printf("Result: %.2f\n", result);
    } else {
        printf("Invalid operation\n");
    }

    return 0;
}