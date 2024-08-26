# Comprehensive Tutorial: Virtualizing the CPU

This tutorial will guide you through the concept of CPU virtualization using a simple program. We'll cover creating the necessary files, compiling the program, running single and multiple instances, understanding the underlying principles, and managing the processes.

## Table of Contents
1. [Creating the Necessary Files](#creating-the-necessary-files)
2. [Compiling the Program](#compiling-the-program)
3. [Running a Single Instance](#running-a-single-instance)
4. [Running Multiple Instances](#running-multiple-instances)
5. [Understanding CPU Virtualization](#understanding-cpu-virtualization)
6. [Managing Processes](#managing-processes)

## Creating the Necessary Files

We need to create two files: `common.h` and `cpu.c`.

1. Create `common.h`:
   Open a text editor and create a file named `common.h` with the following content:

   ```c
   #ifndef __common_h__
   #define __common_h__

   #include <sys/time.h>
   #include <sys/stat.h>
   #include <assert.h>

   double GetTime() {
       struct timeval t;
       int rc = gettimeofday(&t, NULL);
       assert(rc == 0);
       return (double) t.tv_sec + (double) t.tv_usec/1e6;
   }

   void Spin(int howlong) {
       double t = GetTime();
       while ((GetTime() - t) < (double) howlong)
           ; // do nothing in loop
   }

   #endif // __common_h__
   ```

2. Create `cpu.c`:
   Open a text editor and create a file named `cpu.c` with the following content:

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include "common.h"

   int main(int argc, char *argv[])
   {
       if (argc != 2) {
           fprintf(stderr, "usage: cpu <string>\n");
           exit(1);
       }
       char *str = argv[1];
       while (1) {
           printf("%s\n", str);
           Spin(1);
       }
       return 0;
   }
   ```

Ensure both files are in the same directory.

## Compiling the Program

Open a terminal and navigate to the directory containing both files. Then, compile the program using the following command:

```
prompt> gcc -o cpu cpu.c -Wall
```

This command compiles `cpu.c` and creates an executable named `cpu`. The `-Wall` flag enables all compiler warnings.

## Running a Single Instance

1. Run the program by typing:
   ```
   prompt> ./cpu "A"
   ```

2. You'll see output like this:
   ```
   A
   A
   A
   A
   ```
   The program will keep printing "A" once every second.

3. To stop the program, press Ctrl+C.

## Running Multiple Instances

1. Run multiple instances in the background:
   ```
   prompt> ./cpu A & ./cpu B & ./cpu C & ./cpu D &
   ```

2. You'll see output like this:
   ```
   [1] 7353
   [2] 7354
   [3] 7355
   [4] 7356
   A
   B
   D
   C
   A
   B
   D
   C
   A
   ...
   ```

   The numbers (7353, 7354, etc.) are the Process IDs (PIDs) assigned by the operating system.

## Understanding CPU Virtualization

What we're observing here is CPU virtualization in action:

- Despite having only one CPU, all four programs seem to run simultaneously.
- The operating system is creating an illusion of multiple CPUs.
- This is achieved through rapid switching between programs (time-sharing).
- Each program gets a slice of CPU time, creating the appearance of parallel execution.

Key concepts:
- Virtualization: The OS turns a single CPU into seemingly infinite virtual CPUs.
- Scheduling: The OS decides which program to run at any given time.
- Resource Management: The OS fairly distributes CPU time among running programs.

## Managing Processes

Now that we have multiple instances running, let's learn how to manage them:

1. View running background jobs:
   ```
   prompt> jobs -l
   [1]  7353 Running    ./cpu A &
   [2]  7354 Running    ./cpu B &
   [3]  7355 Running    ./cpu C &
   [4]  7356 Running    ./cpu D &
   ```

2. Terminate a specific process using its PID:
   ```
   prompt> kill 7353
   [1]  Terminated: 15  ./cpu A
   ```

3. Forcefully kill a process that's not responding:
   ```
   prompt> kill -9 7354
   [2]  Killed: 9  ./cpu B
   ```

4. Kill all instances of the program at once:
   ```
   prompt> pkill cpu
   [1]  Terminated: 15  ./cpu A
   [2]  Terminated: 15  ./cpu B
   [3]  Terminated: 15  ./cpu C
   [4]  Terminated: 15  ./cpu D
   ```

5. Use job control to terminate processes:
   ```
   prompt> kill %1 %2 %3 %4
   [1]  Terminated: 15  ./cpu A
   [2]  Terminated: 15  ./cpu B
   [3]  Terminated: 15  ./cpu C
   [4]  Terminated: 15  ./cpu D
   ```

Remember:
- Ctrl+C terminates a foreground process.
- `kill` sends a termination signal to a process.
- `kill -9` forcefully terminates a process.
- `pkill` can terminate all instances of a program.
- Using `%n` with `kill` refers to job numbers instead of PIDs.

By following these steps, creating the files, compiling the program, running instances, and managing processes, you've experienced firsthand how the operating system virtualizes the CPU. This allows multiple programs to run concurrently on a single processor, demonstrating the fundamental concept that underlies modern multitasking operating systems. This virtualization enables efficient use of computer resources and provides the illusion of parallel execution to users and applications.

