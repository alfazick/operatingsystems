#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int x = 10;
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child process is running\n");
        sleep(2);
        exit(42); // Upon completion child return with status 42
    } else if (pid > 0) {
        // Parent process
        int status;
        printf("Parent waiting for child\n");
        pid_t child_pid = wait(&status);

        if (WIFEXITED(status)) {
            printf("Child (PID %d) exited with status %d\n",child_pid,WEXITSTATUS(status));
        }


    } else {
        // Fork failed
        perror("fork");
        return 1;
    }
}

