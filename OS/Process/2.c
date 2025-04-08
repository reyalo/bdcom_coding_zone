#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    const char *fifo_path = "/tmp/myfifo_vfork";

    // Create the named pipe (FIFO)
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo");
    }

    pid_t pid = vfork();  // Parent suspends until child _exit or exec

    if (pid < 0) {
        perror("vfork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Detach from terminal (new session)
        setsid();

        // Optional: close stdin/out/err if you want it fully detached
        close(0); close(1); close(2);

        int fd = open(fifo_path, O_RDONLY); // This will block (D state)
        if (fd >= 0) {
            char buf[64];
            read(fd, buf, sizeof(buf));
            close(fd);
        }
        _exit(0);
    }

    // Parent (won't run until child exits due to vfork)
    return 0;
}
