# Core Virtualization Principles in Operating Systems

## CPU Virtualization: Limited Direct Execution (LDE)
* Programs run directly on hardware most of the time (the "direct execution" part)
* This direct execution is "limited" because the OS maintains control at critical points

### Key Points of OS Intervention
1. System Calls
2. Timer Interrupts

### Operational Modes
1. Direct execution mode (efficient, program runs directly on CPU)
2. OS intervention mode (controlled, OS handles special operations)

### Balanced Goals
1. Efficiency: By letting programs run directly on hardware most of the time
2. Control: By maintaining OS oversight at critical moments

## Memory Virtualization
Memory virtualization follows similar principles but with additional requirements:

### Core Requirements
1. Efficiency
   * Must leverage hardware support (registers, TLBs, page tables)
   * Similar to CPU virtualization, direct access when possible

2. Control
   * OS must protect applications from each other
   * OS must protect itself from applications
   * Requires hardware support for memory protection

3. Flexibility
   * Programs should be able to use their address spaces freely
   * Makes system easier to program

### The Crux of Memory Virtualization
How to achieve three competing goals:
1. Build efficient virtualization of memory
2. Provide flexibility needed by applications
3. Maintain control over memory access restrictions
