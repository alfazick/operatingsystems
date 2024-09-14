#include <stdio.h>
#include <unistd.h>

int global_var = 100;

int main() {
    int local_var = 200;
    printf("Exec caller: PID = %d\n", getpid());
    printf("Exec caller: global_var = %d, local_var = %d\n", global_var, local_var);
    printf("Exec caller: Calling exec...\n");
    
    execl("./simple_program", "simple_program", NULL);
    
    perror("execl");
    return 1;
}

