#include <stdio.h>
#include <unistd.h>


void counter_(){
    static int cnt=0;
    cnt++;
    printf("%d\n", cnt);
}



int main() {

    printf("It is parent Process\n");
    pid_t pid = fork();
    // mother process continue executing and get pid > 0, 
    // child process make an identical copy of this code, but executes start from after the fork(); fork() return > 0;

    if (pid > 0) {
        printf("\n\nParent Process: PID = %d\n", getpid());
    } else if (pid == 0) {
        printf("Child Process: PID = %d, Parent PID = %d\n\n", getpid(), getppid());
    } else {
        printf("Fork failed!\n");
    }

    counter_();

    return 0;
}
