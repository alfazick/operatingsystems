#include <stdio.h>
#include <unistd.h>

int new_global_var = 300;

int main() {
    int new_local_var = 400;
    printf("Simple program: PID = %d\n", getpid());
    printf("Simple program: new_global_var = %d, new_local_var = %d\n", new_global_var, new_local_var);
    return 0;
}