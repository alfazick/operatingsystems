# Exec System Call Demonstration

This demonstration shows how the exec system call works in Unix-like operating systems using two C programs.

## Programs

### 1. exec_demo.c

```c
#include <stdio.h>
#include <unistd.h>

int global_var = 100;

int main() {
    int local_var = 200;
    printf("Exec demo: PID = %d\n", getpid());
    printf("Exec demo: global_var = %d, local_var = %d\n", global_var, local_var);
    printf("Exec demo: Calling exec...\n");
    
    execl("./simple_program", "simple_program", NULL);
    
    perror("execl");
    return 1;
}
```

### 2. simple_program.c

```c
#include <stdio.h>
#include <unistd.h>

int new_global_var = 300;

int main() {
    int new_local_var = 400;
    printf("Simple program: PID = %d\n", getpid());
    printf("Simple program: new_global_var = %d, new_local_var = %d\n", new_global_var, new_local_var);
    return 0;
}
```

## How to Run

1. Save the above code into two separate files: `exec_demo.c` and `simple_program.c`.

2. Open a terminal and navigate to the directory containing these files.

3. Compile both programs:
   ```
   gcc -o exec_demo exec_demo.c
   gcc -o simple_program simple_program.c
   ```

4. Run the exec_demo program:
   ```
   ./exec_demo
   ```

## Explanation

When you run `./exec_demo`, the following sequence of events occurs:

1. The `exec_demo` program starts:
   - It prints its Process ID (PID).
   - It displays the values of `global_var` (100) and `local_var` (200).
   - It announces that it's about to call exec.

2. The `execl()` function is called:
   - This function replaces the current process image with a new one.
   - The new process image is loaded from the `simple_program` executable.

3. If `execl()` succeeds:
   - The entire `exec_demo` program (including all its variables and code) is replaced by `simple_program`.
   - `simple_program` begins execution from its `main()` function.
   - It prints its PID (which will be the same as `exec_demo`'s PID).
   - It displays the values of its own `new_global_var` (300) and `new_local_var` (400).

4. If `execl()` fails:
   - The code after the `execl()` call in `exec_demo` would execute.
   - It would print an error message using `perror()`.
   - The program would return with an exit code of 1.

## Key Points

- The process ID remains the same before and after the exec call. This demonstrates that exec doesn't create a new process; it transforms an existing one.
- All memory content of `exec_demo` (including global and local variables) is completely replaced by the memory content of `simple_program`.
- If exec is successful, none of the code after the exec call in `exec_demo` is ever executed.
- The exec call is often used in conjunction with fork to create a new process and then replace it with a different program.

## Memory Layout

Before exec (in exec_demo):
```
+------------------+
|    Stack         |
|    local_var=200 |
|    ...           |
+------------------+
|    Heap          |
|    (empty)       |
+------------------+
|    Data          |
|    global_var=100|
+------------------+
|    Text          |
|    (exec_demo    |
|    code)         |
+------------------+
```

After exec (simple_program is loaded):
```
+------------------+
|    Stack         |
|    new_local_var |
|    =400          |
|    ...           |
+------------------+
|    Heap          |
|    (empty)       |
+------------------+
|    Data          |
|    new_global_var|
|    =300          |
+------------------+
|    Text          |
|    (simple_      |
|    program code) |
+------------------+
```

This demonstration helps visualize how the exec system call works, showing how it completely replaces one program with another within the same process.
