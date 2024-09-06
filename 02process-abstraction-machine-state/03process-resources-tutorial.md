# 🖥️ Comprehensive Tutorial: Examining Process Resources in Linux

## 📚 Table of Contents
1. [Introduction](#introduction)
2. [Sample Program](#sample-program)
3. [Compilation and Execution](#compilation-and-execution)
4. [Examining Process Resources](#examining-process-resources)
   - [Memory Layout](#memory-layout)
   - [File Descriptors](#file-descriptors)
   - [Process Status](#process-status)
   - [Process Limits](#process-limits)
   - [Environment Variables](#environment-variables)
   - [Command Line Arguments](#command-line-arguments)
   - [Threads](#threads)
   - [I/O Statistics](#io-statistics)
5. [Analysis of Results](#analysis-of-results)
6. [Conclusion](#conclusion)

## 1. Introduction 🌟

Understanding the resources allocated to a running process is crucial for developers. This tutorial will guide you through examining various resources the operating system creates for a program, including memory layout, file descriptors, threads, and more.

## 2. Sample Program 💻

We'll use  the `process_demo.c` program that includes file operations to demonstrate file descriptors:

```c
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
```

## 3. Compilation and Execution 🛠️

Compile the program:
```bash
gcc -o process_demo process_demo.c
```

Run the program:
```bash
./process_demo
```

## 4. Examining Process Resources 🔍

While the program is running and paused, we can examine its resources. Replace `<PID>` with the Process ID displayed by the program in the following commands.

### 4.1 Memory Layout 🧠

View the memory map:
```bash
cat /proc/<PID>/maps
```

Use pmap for a more readable format:
```bash
pmap <PID>
```

### 4.2 File Descriptors 📂

List open file descriptors:
```bash
ls -l /proc/<PID>/fd
```

### 4.3 Process Status 📊

View detailed process status:
```bash
cat /proc/<PID>/status
```

### 4.4 Process Limits 📏

Check resource limits:
```bash
cat /proc/<PID>/limits
```

### 4.5 Environment Variables 🌍

View environment variables:
```bash
cat /proc/<PID>/environ | tr '\0' '\n'
```

### 4.6 Command Line Arguments 🖥️

See command line arguments:
```bash
cat /proc/<PID>/cmdline | tr '\0' ' '; echo
```

### 4.7 Threads 🧵

List threads:
```bash
ls -l /proc/<PID>/task
```

### 4.8 I/O Statistics 📈

Check I/O statistics:
```bash
cat /proc/<PID>/io
```

## 5. Analysis of Results 📝

| Resource | Description |
|----------|-------------|
| Memory Layout | • Text segment: Executable code  |
|              | • Data segment: Initialized global variables |
|              | • BSS: Uninitialized global variables |
|              | • Heap: Dynamic memory allocation |
|              | • Stack: Local variables and function calls |
| File Descriptors | • 0, 1, 2: Standard input, output, error |
|                  | • Additional FDs for opened files |
| Process Status | • State: Running, sleeping, etc. |
|                | • Memory usage: VmSize, VmRSS, etc. |
|                | • Threads count |
| Resource Limits | • Max open files, max stack size, etc. |
| Environment Variables | • PATH, HOME, and other environment settings |
| Command Line Arguments | • How the program was invoked |
| Threads | • Number of threads and their IDs |
| I/O Statistics | • Read/write counts, bytes read/written |

## 6. Conclusion 🎓

This tutorial demonstrated how to examine various resources allocated to a running process in Linux. Understanding these resources is crucial for:
- 🐞 Debugging
- 🚀 Performance optimization
- 🔒 Security analysis
- 📊 System resource management

Remember that the `/proc` filesystem provides a wealth of information about running processes, and exploring it can give you deep insights into process behavior and resource utilization.

