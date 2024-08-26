# Comprehensive Tutorial: Virtualizing Memory and Address Space Randomization

This tutorial will guide you through the concept of memory virtualization using a simple program. We'll cover creating the necessary files, compiling the program, managing address space randomization, running single and multiple instances, and understanding the underlying principles of memory virtualization.

## Table of Contents
1. [Understanding Physical Memory](#understanding-physical-memory)
2. [Creating the Necessary Files](#creating-the-necessary-files)
3. [Compiling the Program](#compiling-the-program)
4. [Managing Address Space Randomization](#managing-address-space-randomization)
5. [Running a Single Instance](#running-a-single-instance)
6. [Running Multiple Instances](#running-multiple-instances)
7. [Understanding Memory Virtualization](#understanding-memory-virtualization)
8. [Restoring Address Space Randomization](#restoring-address-space-randomization)

## Understanding Physical Memory

Before we dive into virtualization, let's briefly discuss physical memory:

- Memory is essentially an array of bytes.
- To read memory, you specify an address to access the data stored there.
- To write or update memory, you specify both the address and the data to be written.
- Memory is accessed constantly during program execution, both for data and instructions.

## Creating the Necessary Files

We need to create two files: `common.h` and `mem.c`.

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

2. Create `mem.c`:
   Open a text editor and create a file named `mem.c` with the following content:

   ```c
   #include <unistd.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include "common.h"

   int main(int argc, char *argv[]) {
       if (argc != 2) {
           fprintf(stderr, "usage: mem <value>\n");
           exit(1);
       }
       int *p;
       p = malloc(sizeof(int));  // a1
       assert(p != NULL);
       printf("(%d) addr pointed to by p: %p\n", (int) getpid(), p);  // a2
       *p = atoi(argv[1]);  // a3: assign value to addr stored in p
       while (1) {
           Spin(1);  // a4
           *p = *p + 1;
           printf("(%d) value of p: %d\n", getpid(), *p);
       }
       return 0;
   }
   ```

Ensure both files are in the same directory.

## Compiling the Program

Open a terminal and navigate to the directory containing both files. Then, compile the program using the following command:

```
prompt> gcc -o mem mem.c -Wall
```

This command compiles `mem.c` and creates an executable named `mem`. The `-Wall` flag enables all compiler warnings.

## Managing Address Space Randomization

Address Space Layout Randomization (ASLR) is a security feature that randomizes the memory addresses used by key areas of a process. For our experiment, we need to temporarily disable it.

1. Check the current ASLR setting:
   ```
   prompt> cat /proc/sys/kernel/randomize_va_space
   ```
   You'll likely see `2`, which means full randomization.

2. Disable ASLR:
   ```
   prompt> sudo sh -c 'echo 0 > /proc/sys/kernel/randomize_va_space'
   ```
   You'll be prompted for your password.

3. Verify the change:
   ```
   prompt> cat /proc/sys/kernel/randomize_va_space
   ```
   You should now see `0`.

Note: Disabling ASLR makes your system more vulnerable to certain types of attacks. We'll re-enable it after our experiment.

## Running a Single Instance

1. Run the program by typing:
   ```
   prompt> ./mem 0
   ```

2. You'll see output like this:
   ```
   (2134) address pointed to by p: 0x200000
   (2134) value of p: 1
   (2134) value of p: 2
   (2134) value of p: 3
   (2134) value of p: 4
   (2134) value of p: 5
   ```

   The program will continue incrementing and printing the value indefinitely.

3. To stop the program, press Ctrl+C.

## Running Multiple Instances

1. Open two terminal windows or tabs.

2. In each terminal, navigate to the directory containing the `mem` executable.

3. In the first terminal, run:
   ```
   prompt> ./mem 0
   ```

4. In the second terminal, run:
   ```
   prompt> ./mem 0
   ```

5. Observe the output in both terminals. You should see something like this:

   Terminal 1:
   ```
   (24113) address pointed to by p: 0x200000
   (24113) value of p: 1
   (24113) value of p: 2
   (24113) value of p: 3
   ...
   ```

   Terminal 2:
   ```
   (24114) address pointed to by p: 0x200000
   (24114) value of p: 1
   (24114) value of p: 2
   (24114) value of p: 3
   ...
   ```

6. To stop the programs, press Ctrl+C in each terminal.

## Understanding Memory Virtualization

What we're observing here demonstrates memory virtualization:

1. Each process seems to allocate memory at the same address (0x200000).
2. Each process independently updates the value at this address.
3. The changes in one process do not affect the other process.

Key concepts:

- Virtual Address Space: Each process has its own private virtual address space.
- Memory Mapping: The OS maps these virtual addresses onto the physical memory of the machine.
- Isolation: Memory references within one program do not affect the address space of other processes or the OS itself.
- Illusion of Exclusive Memory: Each process behaves as if it has the entire physical memory to itself.

The reality is that physical memory is a shared resource managed by the operating system. The OS creates the illusion of a private, contiguous address space for each process, while behind the scenes it manages the mapping of these virtual addresses to physical memory locations.

## Restoring Address Space Randomization

After completing your experiments, it's crucial to re-enable ASLR to maintain system security.

1. Re-enable ASLR:
   ```
   prompt> sudo sh -c 'echo 2 > /proc/sys/kernel/randomize_va_space'
   ```

2. Verify the change:
   ```
   prompt> cat /proc/sys/kernel/randomize_va_space
   ```
   You should now see `2` again.

The values for randomize_va_space are:
- 0: No randomization
- 1: Conservative randomization
- 2: Full randomization (default on most systems)

By running these examples and observing the behavior of multiple instances of the `mem` program, you've experienced firsthand how the operating system virtualizes memory, allowing multiple programs to seemingly use the same memory addresses independently. This fundamental concept underlies modern multitasking operating systems, enabling efficient and secure use of physical memory resources.

Remember, while disabling ASLR can be useful for educational purposes or debugging, it should always be re-enabled in normal circumstances to maintain system security.

