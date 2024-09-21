#include <unistd.h>

int main() {
    const char *msg = "Hello from the user process!\n";
    write(1, msg, 28);
    return 0;
}