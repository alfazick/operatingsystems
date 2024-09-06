#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int global_var = 42;

void demonstrate_stack(int level) {
    int local_var = level;
    printf("Stack level: %d, Address of local_var: %p\n", level, (void*)&local_var);
    
    if (level < 3) {
        demonstrate_stack(level + 1);
    }
    
    printf("Press Enter to continue (level %d)...", level);
    getchar();
}

int main() {
    printf("Process ID: %d\n", getpid());
    printf("Address of main function: %p\n", (void*)main);
    printf("Address of global variable: %p\n", (void*)&global_var);

    // Open a file to demonstrate file descriptors
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0644);
    if (fd != -1) {
        printf("Opened file descriptor: %d\n", fd);
    }

    printf("Press Enter to start resource examination...");
    getchar();

    demonstrate_stack(0);

    if (fd != -1) {
        close(fd);
    }

    printf("Resource demonstration complete.\n");
    printf("Press Enter to exit...");
    getchar();
    return 0;
}