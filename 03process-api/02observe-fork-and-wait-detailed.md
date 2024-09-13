# Detailed Guide: Observing Fork and Wait Behavior

This guide provides a comprehensive approach to using GDB (GNU Debugger) for observing the behavior of `fork()` and `wait()` system calls in a simple C program.

## The Program

```c
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
            printf("Child (PID %d) exited with status %d\n", child_pid, WEXITSTATUS(status));
        }
    } else {
        // Fork failed
        perror("fork");
        return 1;
    }
}
```

## Debugging Setup

1. Compile the program with debugging symbols:
   ```
   gcc -g fork_wait_demo.c -o fork_wait_demo
   ```

2. Start GDB:
   ```
   gdb ./fork_wait_demo
   ```

3. Set breakpoints and configure fork following:
   ```
   (gdb) break 9
   (gdb) break 11
   (gdb) break 13
   (gdb) set follow-fork-mode parent
   (gdb) set detach-on-fork off
   (gdb) set print entry-values no
   ```

### Explanation of GDB Settings

- `set follow-fork-mode parent`: This tells GDB to continue debugging the parent process after a fork. By default, GDB follows the child process.
- `set detach-on-fork off`: This keeps both parent and child processes under GDB's control after a fork. By default, GDB detaches from one process (whichever it's not following).
- `set print entry-values no`: This makes GDB display exit codes in decimal, matching what we see in the program's output.

These settings allow us to switch between and debug both the parent and child processes, giving us a complete picture of what's happening during and after the fork.

## Debugging Steps

1. Run the program:
   ```
   (gdb) run
   ```

2. At the first breakpoint (line 9, right after fork), examine `pid` and `x` in the parent:
   ```
   (gdb) print pid
   (gdb) print x
   ```

3. Use the `info inferiors` command to see all processes under GDB's control:
   ```
   (gdb) info inferiors
   ```
   This will show you both the parent and child processes.

4. Switch to the child process:
   ```
   (gdb) inferior 2
   ```

5. Continue the child process to its first breakpoint (line 9 in the child):
   ```
   (gdb) continue
   ```

6. In the child process, print `pid` and `x`:
   ```
   (gdb) print pid
   (gdb) print x
   ```

7. Continue the child process:
   ```
   (gdb) continue
   ```

8. The child will print its message, sleep, and then stop at the exit. Step into the exit function:
   ```
   (gdb) step
   ```

9. Switch back to the parent and continue:
   ```
   (gdb) inferior 1
   (gdb) continue
   ```

10. The parent will print its message, wait for the child, and then complete.

## Key Observations

- The `info inferiors` command shows all processes under GDB's control. This is crucial for understanding the state of both parent and child processes during debugging.
- In the parent, `pid` will be the PID of the child process.
- In the child, `pid` will be 0.
- The value of `x` should be the same (10) in both parent and child, demonstrating that memory is copied during fork.
- The child process exits with status 42.
- The parent successfully waits for the child and retrieves its exit status.

## Conclusion

This debugging session provides deep insight into the mechanics of process creation and synchronization in Unix-like operating systems. By using GDB with the appropriate settings, we can observe:

1. How `fork()` creates an exact copy of the parent process, including all variables.
2. The difference in `pid` values between parent and child immediately after forking.
3. How `wait()` in the parent blocks until the child process terminates.
4. How the exit status of the child is passed to and retrieved by the parent.

Understanding these mechanisms is crucial for developing and debugging multi-process applications in Unix-like environments.
