// Define necessary types
typedef unsigned long size_t;
typedef unsigned int mode_t;

// Define necessary constants
#define O_RDONLY    0
#define O_WRONLY    1
#define O_CREAT     64
#define O_TRUNC     512

// System call numbers
#define SYS_WRITE   1
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_READ    0

long my_write(int fd, const void *buf, size_t count) {
    long ret;
    __asm__ (
        "mov $1, %%rax\n\t"  // SYS_WRITE
        "mov %1, %%rdi\n\t"  // file descriptor
        "mov %2, %%rsi\n\t"  // buffer
        "mov %3, %%rdx\n\t"  // count
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r" (ret)
        : "r" ((long)fd), "r" (buf), "r" ((long)count)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    return ret;
}

long my_open(const char *pathname, int flags, mode_t mode) {
    long ret;
    __asm__ (
        "mov $2, %%rax\n\t"  // SYS_OPEN
        "mov %1, %%rdi\n\t"  // pathname
        "mov %2, %%rsi\n\t"  // flags
        "mov %3, %%rdx\n\t"  // mode
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r" (ret)
        : "r" (pathname), "r" ((long)flags), "r" ((long)mode)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    return ret;
}

long my_read(int fd, void *buf, size_t count) {
    long ret;
    __asm__ (
        "mov $0, %%rax\n\t"  // SYS_READ
        "mov %1, %%rdi\n\t"  // file descriptor
        "mov %2, %%rsi\n\t"  // buffer
        "mov %3, %%rdx\n\t"  // count
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r" (ret)
        : "r" ((long)fd), "r" (buf), "r" ((long)count)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    return ret;
}

long my_close(int fd) {
    long ret;
    __asm__ (
        "mov $3, %%rax\n\t"  // SYS_CLOSE
        "mov %1, %%rdi\n\t"  // file descriptor
        "syscall\n\t"
        "mov %%rax, %0"
        : "=r" (ret)
        : "r" ((long)fd)
        : "%rax", "%rdi"
    );
    return ret;
}

int main() {
    const char *filename = "hello.txt";
    const char *message = "Hello, World!\n";
    char buffer[100];
    long fd, bytes_written, bytes_read;

    // Open file for writing
    fd = my_open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        my_write(2, "Error opening file for writing\n", 31);
        return 1;
    }

    // Write to the file
    bytes_written = my_write(fd, message, 14);
    if (bytes_written != 14) {
        my_write(2, "Error writing to file\n", 22);
        return 1;
    }

    // Close the file
    my_close(fd);

    // Open file for reading
    fd = my_open(filename, O_RDONLY, 0);
    if (fd == -1) {
        my_write(2, "Error opening file for reading\n", 31);
        return 1;
    }

    // Read from the file
    bytes_read = my_read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        my_write(2, "Error reading from file\n", 24);
        return 1;
    }

    // Null-terminate the buffer
    buffer[bytes_read] = '\0';

    // Write the contents to stdout
    my_write(1, "File contents: ", 15);
    my_write(1, buffer, bytes_read);

    // Close the file
    my_close(fd);

    return 0;
}