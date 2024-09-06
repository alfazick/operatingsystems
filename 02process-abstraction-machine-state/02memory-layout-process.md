# 🧠 Comprehensive Tutorial: Examining Memory Layout of a Loaded Program

## 📚 Table of Contents
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Sample Program](#sample-program)
4. [Compilation and Preparation](#compilation-and-preparation)
5. [GDB Analysis](#gdb-analysis)
   - [Examining Memory Areas](#examining-memory-areas)
6. [Analysis of Results](#analysis-of-results)
7. [Conclusion](#conclusion)

## 1. Introduction 🌟

Understanding the memory layout of a running program is crucial for developers, especially those working on performance optimization, debugging complex issues, or developing low-level software. This tutorial provides a step-by-step guide to examining the memory layout of a C program using the GNU Debugger (GDB).

## 2. Prerequisites 🛠️

Before we begin, make sure you have the following:

- A Linux-based operating system
- GCC (GNU Compiler Collection)
- GDB (GNU Debugger)
- Basic knowledge of C programming
- Familiarity with command-line interfaces

## 3. Sample Program 💻

We'll use the following C program to demonstrate various memory areas:

```c
#include <stdio.h>
#include <stdlib.h>

// Global variables (stored in Data Segment - Initialized)
int global_initialized = 100;

// Global variables (stored in BSS Segment - Uninitialized)
int global_uninitialized;

// Function to demonstrate stack usage
void stack_demo(int param) {
    // Local variables (stored on Stack)
    int local_var = 20;
    printf("Stack Demo - Parameter: %d, Local Variable: %d\n", param, local_var);
}

// Static variable (stored in Data Segment)
static int static_var = 50;

int main() {
    // Code Segment
    printf("This print statement's code is in the Code Segment\n");

    // Data Segment - Static Area
    printf("Static Variable: %d\n", static_var);

    // Data Segment - Initialized
    printf("Global Initialized: %d\n", global_initialized);

    // BSS Segment
    printf("Global Uninitialized (BSS): %d\n", global_uninitialized);

    // Stack
    stack_demo(10);

    // Heap
    int* heap_var = (int*)malloc(sizeof(int));
    *heap_var = 30;
    printf("Heap Variable: %d\n", *heap_var);

    // Don't forget to free heap memory
    free(heap_var);

    return 0;
}
```

Save this code as `memory_areas_demo.c`.

## 4. Compilation and Preparation 🔧

1. Compile the program with debugging symbols:
   ```bash
   gcc -g memory_areas_demo.c -o memory_areas_demo
   ```

2. Disable Address Space Layout Randomization (ASLR) for consistent results:
   ```bash
   echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
   ```
   
   Verify that ASLR is disabled:
   ```bash
   cat /proc/sys/kernel/randomize_va_space
   ```
   It should output 0.

## 5. GDB Analysis 🔍

Start GDB:
```bash
gdb ./memory_areas_demo
```

Set breakpoints:
```gdb
(gdb) break main
(gdb) break stack_demo
(gdb) break 39
```

Run the program:
```gdb
(gdb) run
```

### Examining Memory Areas 🔬

When stopped at `main`:

1. **Text (Code) Segment:**
   ```gdb
   (gdb) disassemble main
   ```
   Output (partial):
   ```
   Dump of assembler code for function main:
      0x00005555555551db <+0>:     endbr64 
      0x00005555555551df <+4>:     push   %rbp
      0x00005555555551e0 <+5>:     mov    %rsp,%rbp
      ...
   ```

2. **Data Segment (Initialized Data):**
   ```gdb
   (gdb) print &global_initialized
   (gdb) x/d &global_initialized
   (gdb) print &static_var
   (gdb) x/d &static_var
   ```
   Output:
   ```
   $1 = (int *) 0x555555558010 <global_initialized>
   0x555555558010 <global_initialized>:    100
   $2 = (int *) 0x555555558014 <static_var>
   0x555555558014 <static_var>:    50
   ```

3. **BSS Segment (Uninitialized Data):**
   ```gdb
   (gdb) print &global_uninitialized
   (gdb) x/d &global_uninitialized
   ```
   Output:
   ```
   $3 = (int *) 0x55555555801c <global_uninitialized>
   0x55555555801c <global_uninitialized>:  0
   ```

4. **Overall Memory Map:**
   ```gdb
   (gdb) info proc mappings
   ```
   Output:
   ```
   process 5675
   Mapped address spaces:

             Start Addr           End Addr       Size     Offset objfile
         0x555555554000     0x555555555000     0x1000        0x0 /home/student/memory/memory_areas_demo
         0x555555555000     0x555555556000     0x1000     0x1000 /home/student/memory/memory_areas_demo
         0x555555556000     0x555555557000     0x1000     0x2000 /home/student/memory/memory_areas_demo
         0x555555557000     0x555555558000     0x1000     0x2000 /home/student/memory/memory_areas_demo
         0x555555558000     0x555555559000     0x1000     0x3000 /home/student/memory/memory_areas_demo
         ...
         0x7ffffffde000     0x7ffffffff000    0x21000        0x0 [stack]
     0xffffffffff600000 0xffffffffff601000     0x1000        0x0 [vsyscall]
   ```

Continue to `stack_demo`:
```gdb
(gdb) continue
```

5. **Stack:**
   ```gdb
   (gdb) info args
   (gdb) info locals
   (gdb) x/32x $sp
   ```
   Output:
   ```
   param = 10
   local_var = 20
   0x7fffffffe398: 0x55555248      0x00005555      0xffffe4a0      0x00007fff
   0x7fffffffe3a8: 0x00000000      0x00000000      0x00000000      0x00000000
   ...
   ```

Continue to the heap allocation:
```gdb
(gdb) continue
```

6. **Heap:**
   ```gdb
   (gdb) print heap_var
   (gdb) x/d heap_var
   ```
   Output:
   ```
   $4 = (int *) 0x5555555596b0
   0x5555555596b0: 30
   ```

7. **Final Memory Map:**
   ```gdb
   (gdb) info proc mappings
   ```
   Output:
   ```
   process 5675
   Mapped address spaces:

             Start Addr           End Addr       Size     Offset objfile
         0x555555554000     0x555555555000     0x1000        0x0 /home/student/memory/memory_areas_demo
         ...
         0x555555559000     0x55555557a000    0x21000        0x0 [heap]
         ...
         0x7ffffffde000     0x7ffffffff000    0x21000        0x0 [stack]
     0xffffffffff600000 0xffffffffff601000     0x1000        0x0 [vsyscall]
   ```
## 6. Analysis of Results 📊

In a typical program's memory layout, from low to high addresses:

```
Low Address
+------------------+
|   Text Segment   | <-- Code
+------------------+
|   Data Segment   | <-- Initialized Data
+------------------+
|   BSS Segment    | <-- Uninitialized Data
+------------------+
|                  |
|       Heap       |
|        ↑         |
|                  |
+------------------+
|    Libraries     |
+------------------+
|                  |
|       Stack      |
|        ↓         |
|                  |
+------------------+
| Virtual Syscalls |
+------------------+
High Address
```

Let's break down each segment from low to high addresses:

1. **Text Segment (Code):** 
   - Located at the lowest address
   - Contains the executable code of the program
   - In our example: The machine code for all functions
   - Read-only to prevent accidental modification of instructions

2. **Data Segment:** 
   - Contains initialized global and static variables
   - In our example: `global_initialized` and `static_var`
   - Read-write, as these variables can be modified during program execution

3. **BSS Segment:** 
   - Contains uninitialized global and static variables (initialized to zero by the system)
   - In our example: `global_uninitialized`
   - Read-write, similar to the Data Segment

4. **Heap:** 
   - Grows upwards (↑) towards higher memory addresses
   - Used for dynamic memory allocation (e.g., malloc)
   - In our example: `heap_var`
   - Managed by the programmer (allocation and deallocation)

5. **Libraries:** 
   - Shared libraries are loaded into memory
   - Located between the heap and stack
   - Allows code sharing between multiple processes

6. **Stack:** 
   - Grows downwards (↓) towards lower memory addresses
   - Used for local variables, function parameters, and return addresses
   - In our example: `param` and `local_var` in the `stack_demo` function
   - LIFO (Last In, First Out) structure

7. **Virtual System Calls:**
   - Located at the highest address
   - Used for fast system calls
   - Provided by the kernel for efficient execution of frequently used system calls

Key observations:
- **Memory Layout:** The layout progresses from Text Segment at the lowest address to Virtual Syscalls at the highest address.
- **Growth Directions:** The heap grows upwards while the stack grows downwards, efficiently using the available memory between them.
- **Static vs Dynamic:** Text, Data, and BSS segments are static (fixed size), while Heap and Stack are dynamic (can grow or shrink).
- **Data and BSS:** Initialized (Data) and uninitialized (BSS) global variables are in adjacent memory regions for efficiency.
- **Shared Libraries:** Are loaded into memory between the heap and stack, allowing them to be shared among multiple processes.
- **Memory Protection:** Different segments have different access permissions (e.g., Text is typically read-only, while Data and BSS are read-write).
- **Consistency:** With ASLR disabled, these addresses remain consistent across multiple runs of the program, which is useful for debugging and understanding memory layout.

## 7. Conclusion 🎓

This tutorial has demonstrated how to examine the memory layout of a loaded program using GDB. We've explored various memory segments including the text, data, BSS, heap, and stack. Understanding these concepts is crucial for advanced programming, debugging, and optimizing applications.

**P.S Remember to re-enable ASLR after your analysis:**
```bash
echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
```