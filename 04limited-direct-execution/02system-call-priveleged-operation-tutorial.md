# Comprehensive System Call Tutorial in C

This tutorial provides a step-by-step guide to understanding and implementing system calls in C, progressing from basic usage to advanced, low-level implementations.

## Table of Contents

1. [Part 1: Basic System Call Usage](#part-1-basic-system-call-usage)
2. [Part 2: Observing System Calls with strace](#part-2-observing-system-calls-with-strace)
3. [Part 3: Using Inline Assembly for System Calls](#part-3-using-inline-assembly-for-system-calls)
4. [Part 4: Advanced Implementation - No External Headers](#part-4-advanced-implementation---no-external-headers)

## Part 1: Basic System Call Usage

We'll start with a simple C program that makes a system call to write a message to the terminal.

### Step 1.1: Write the C Program

Create a file called `simple_syscall.c` with the following content:

```c
#include <unistd.h>

int main() {
    const char *msg = "Hello from the user process!\n";
    write(1, msg, 28);
    return 0;
}
```

### Step 1.2: Compile the Program

Compile the program using:

```bash
gcc -o simple_syscall simple_syscall.c
```

### Step 1.3: Run the Program

Run the compiled program:

```bash
./simple_syscall
```

You should see the message "Hello from the user process!" printed on the terminal.

## Part 2: Observing System Calls with strace

Now, we'll use `strace` to observe the system calls made by our program.

### Step 2.1: Use strace to Trace System Calls

Run the program with `strace` to capture system calls:

```bash
strace ./simple_syscall
```

You'll see output similar to this:

```
execve("./simple_syscall", ["./simple_syscall"], 0x7ffd123456780 /* 24 vars */) = 0
write(1, "Hello from the user process!\n", 28) = 28
exit_group(0)                           = ?
+++ exited with 0 +++
```

This shows the `write` system call being invoked with its arguments and return value.

## Part 3: Using Inline Assembly for System Calls

Next, we'll use inline assembly to make the same `write` system call directly.

### Step 3.1: Write Inline Assembly Code

Create a file called `syscall_asm.c` with the following content:

```c
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
```

### Step 3.2: Compile and Run the Program

Compile and run it:

```bash
gcc -o syscall_asm syscall_asm.c
./syscall_asm
```

## Part 4: Advanced Implementation - No External Headers

Finally, we'll create a more complex program that uses multiple system calls (open, write, read, close) without relying on any external headers.

### Step 4.1: Write the C Program

Create a file called `syscall_functions_no_headers.c` with the following content:

```c
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
```

### Step 4.2: Compile the Program

Compile the program using:

```bash
gcc -o syscall_functions_no_headers syscall_functions_no_headers.c
```

### Step 4.3: Run the Program

Run the compiled program:

```bash
./syscall_functions_no_headers
```

You should see output like this:

```
File contents: Hello, World!
```

Additionally, a file named `hello.txt` will be created in the same directory, containing the text "Hello, World!".

### Step 4.4: Observe System Calls with strace

To see all the system calls in action, run the program with strace:

```bash
strace ./syscall_functions_no_headers
```

This will show you all the system calls made by the program, including open, write, read, and close.

## Conclusion

This tutorial has covered:

1. Basic usage of system calls in C
2. Observing system calls using `strace`
3. Using inline assembly to make system calls directly
4. Creating a complex program using multiple system calls without external headers

These examples provide a deep, low-level understanding of how programs interact with the operating system through system calls, while also demonstrating good coding practices like modularity and reusability. The final version shows how to create a completely self-contained program that interacts directly with the operating system.

