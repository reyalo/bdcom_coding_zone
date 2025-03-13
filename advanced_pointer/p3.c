
for(int i=0; i++){}s
1. Dynamic 2D Arrays
    int **matrix = malloc(rows * sizeof(int *));
    for(int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
    }




// 1. Dynamic 2D Arrays
    int **matrix = malloc(rows * sizeof(int *));
    for(int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
    }


//2. Modifying Pointer in Functions
    void modify_ptr(int **ptr) {
        *ptr = malloc(sizeof(int));
        **ptr = 10;
    }


//3. String Arrays (Command Line Args)
    int main(int argc, char **argv) {
        // char *names[] = {"John", "Jane", "Bob"};
    }


//4. Linked List Operations
    void insert(Node **head, int data) {
        Node *new = malloc(sizeof(Node));
        new->next = *head;
        *head = new;
    }


//5. Memory Management
    void free_array(int **arr, int rows) {
        for(int i = 0; i < rows; i++) {
            free(arr[i]);
        }
        free(arr);
    }



//6. Buffer Management
    void resize_buffer(char **buf, size_t *size) {
        *buf = realloc(*buf, *size * 2);
        *size *= 2;
    }
