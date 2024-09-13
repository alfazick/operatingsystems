# Using GDB to Examine fork() Behavior

This guide provides step-by-step instructions on how to use GDB (GNU Debugger) to examine the behavior of the `fork()` system call in C programs.

## Step 1: Prepare the C Program

Create a simple C program that demonstrates the use of `fork()`:

```c
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
```

Save this as `fork_demo.c`.

## Step 2: Compile the Program

Compile the program with debugging symbols:

```bash
gcc -g fork_demo.c -o fork_demo
```

## Step 3: Start GDB

Launch GDB with your compiled program:

```bash
gdb ./fork_demo
```

## Step 4: Set Up GDB for fork()

In the GDB prompt, enter these commands:

```
(gdb) break 6
(gdb) set follow-fork-mode child
(gdb) set detach-on-fork off
```

These commands:
1. Set a breakpoint after the `fork()` call.
2. Tell GDB to follow the child process after forking.
3. Keep both parent and child processes under GDB's control.

## Step 5: Run the Program

Start the program:

```
(gdb) run
```

## Step 6: Examine Processes

After hitting the breakpoint, use these commands to examine both processes:

```
(gdb) info inferior
(gdb) inferior 1
(gdb) print pid
(gdb) inferior 2
(gdb) print pid
```

This will show you:
- Information about both processes
- The value of `pid` in the parent process (should be the child's PID)
- The value of `pid` in the child process (should be 0)

## Step 7: Continue Execution

To see the program's output, continue execution for both processes:

```
(gdb) continue
(gdb) inferior 1
(gdb) continue
```

## Step 8: Exit GDB

After both processes have completed, exit GDB:

```
(gdb) quit
```

By following these steps, you can directly observe how `fork()` behaves in both the parent and child processes, confirming that the child receives 0 as the return value, while the parent receives the child's PID.
