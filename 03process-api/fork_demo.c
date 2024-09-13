#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
    pid_t pid = fork();
    // set a break point here
    // Pause the process to prevent it from exiting immediately
    getchar();  // Waits for user input
    
    printf("Process %d: fork() returns %d\n", getpid(),pid);


    return 0;
}