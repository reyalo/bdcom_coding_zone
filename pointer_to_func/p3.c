#include<stdio.h>
#include<string.h>

typedef struct Person
{
    char name[50];
    int age;
} person_t;

// Define a structure to store function info
typedef struct {
    char type[20];
    void (*print_func)(void*);
} func_info_t;

// Global array of function infos
func_info_t infos[10];
int info_count = 0;

void print_int(void* data){
    int* value = (int*)data;
    printf("%d\n", *value);
}

void print_string(void* data){
    char* str = (char*)data;
    printf("%s\n", str);
}

void print_person(void* data){
    person_t* person = (person_t*)data;
    printf("Name: %s, Age: %d\n", person->name, person->age);
}

void register_info(const char* type, void (*func)(void*)){
    strcpy(infos[info_count].type, type);
    infos[info_count].print_func = func;
    info_count++;
}

void print_text(void* data, const char* type){
    for(int i = 0; i < info_count; i++){
        if(strcmp(infos[i].type, type) == 0){
            infos[i].print_func(data);
            return;
        }
    }
    printf("Unknown type: %s\n", type);
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