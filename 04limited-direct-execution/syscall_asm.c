int main() {
    const char *msg = "Hello from the user process!\n";
    long syscall_number = 1;  // Syscall number for write is 1
    long file_descriptor = 1; // Standard output (stdout)
    long message_length = 28; // Length of the message

    __asm__ (
        "mov %0, %%rax\n\t"  // Move the syscall number (1) into rax
        "mov %1, %%rdi\n\t"  // Move the file descriptor (1) into rdi
        "mov %2, %%rsi\n\t"  // Move the message pointer into rsi
        "mov %3, %%rdx\n\t"  // Move the message length into rdx
        "syscall"            // Trigger the syscall
        :
        : "r"(syscall_number), "r"(file_descriptor), "r"(msg), "r"(message_length)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );

    return 0;
}