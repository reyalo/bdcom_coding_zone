void print_char(const char *str, size_t sz){

    printf("\n");
    for(int i = 0; i < sz; i++){
        printf("%u ", str[i]);
    }
    printf("\n");
}

void print_charc(const char *str, size_t sz){
    printf("\n");
    for(int i = 0; i < sz; i++){
        printf("%c ", str[i]);
    }
    printf("\n");
}