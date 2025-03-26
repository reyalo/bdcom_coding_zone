#include<stdio.h>

typedef struct Person
{
    char name[50];
    int age;

} person_t;


void print_int(){

}

void print_string(){

}

void print_person(){

}


void main(void)
{
    int int_a = 10;
    char str1[] = "bdcom";
    person_t person = {"Tom", 18};

    memset(&infos, 0, sizeof(infos));

    register_info("int", print_int);
    register_info("string", print_string);
    register_info("person_t", print_person);

    print_text(&int_a, "int");
    print_text(&str1, "string");
    print_text(&person, "person_t");

    return;
}