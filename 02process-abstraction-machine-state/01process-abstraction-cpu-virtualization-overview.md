
Note: "Markdown Preview Mermaid Support" install extension

# Process Abstraction and CPU Virtualization: A Schematic Overview

## 1. The Process Abstraction

- **Process**: A running instance of a program
- **Components**: Program counter, memory/address space, registers, open files, I/O information

```mermaid
graph TD
    A[Program] -->|OS loads into memory| B[Process]
    B --> C[Program Counter]
    B --> D[Memory/Address Space]
    B --> E[Registers]
    B --> F[Open Files]
    B --> G[I/O Information]
```

## 2. CPU Virtualization

- **Illusion**: Many virtual CPUs from limited physical CPUs
- **Technique**: Time-sharing

```mermaid
graph TD
    A[Physical CPU] -->|Time-sharing| B[Virtual CPU 1]
    A -->|Time-sharing| C[Virtual CPU 2]
    A -->|Time-sharing| D[Virtual CPU 3]
    B --> E[Process 1]
    C --> F[Process 2]
    D --> G[Process 3]
```

## 3. Time-sharing Mechanism

- **Context Switch**: Saving and restoring process state
- **Time Slice**: Fixed amount of time each process runs

```mermaid
sequenceDiagram
    participant OS as Operating System
    participant CPU as CPU
    participant P1 as Process 1
    participant P2 as Process 2
    participant P3 as Process 3

    OS->>CPU: Run P1
    CPU->>P1: Execute
    Note over CPU,P1: Time slice expires
    OS->>CPU: Context Switch
    CPU->>P2: Execute
    Note over CPU,P2: Time slice expires
    OS->>CPU: Context Switch
    CPU->>P3: Execute
    Note over CPU,P3: Time slice expires
    OS->>CPU: Context Switch
    CPU->>P1: Resume Execution
```

## 4. Process States

- **States**: New, Ready, Running, Waiting, Terminated
- **Transitions**: Controlled by the OS scheduler

```mermaid
stateDiagram-v2
    [*] --> New
    New --> Ready: Admitted
    Ready --> Running: Scheduler Dispatch
    Running --> Ready: Interrupt
    Running --> Waiting: I/O or Event Wait
    Waiting --> Ready: I/O or Event Completion
    Running --> Terminated: Exit
    Terminated --> [*]
```

## 5. CPU Scheduling

- **Short-term**: Selects which process to execute next
- **Medium-term**: Handles swapping processes between memory and disk
- **Long-term**: Controls which jobs are admitted to the system

```mermaid
graph TD
    A[CPU Scheduler] --> B[Short-term Scheduler]
    A --> C[Medium-term Scheduler]
    A --> D[Long-term Scheduler]
    B --> E[Ready Queue]
    C --> F[Swap In/Out]
    D --> G[Job Queue]
```

## 6. Scheduling Policies

- **Goal**: Optimize various metrics (throughput, response time, fairness)
- **Trade-offs**: Different policies optimize for different scenarios

```mermaid
graph TD
    A[Scheduling Policies] --> B[First-Come, First-Served FCFS]
    A --> C[Shortest Job First SJF]
    A --> D[Priority Scheduling]
    A --> E[Round Robin RR]
    A --> F[Multilevel Queue]
    A --> G[Multilevel Feedback Queue]
```

## 7. Mechanisms vs Policies

- **Mechanisms**: Low-level implementations (how)
- **Policies**: High-level decisions (what)

```mermaid
graph TD
    A[Operating System] --> B[Mechanisms]
    A --> C[Policies]
    B --> D[Context Switch]
    B --> E[Process Creation]
    B --> F[Process Termination]
    C --> G[Scheduling Algorithm]
    C --> H[Memory Allocation Strategy]
    C --> I[I/O Management]
```

## 8. Process Control Block (PCB)

- **PCB**: Data structure maintaining all information about a process
- **Usage**: Essential for context switching and process management

```mermaid
graph TD
    A[Process Control Block] --> B[Process ID]
    A --> C[Process State]
    A --> D[Program Counter]
    A --> E[CPU Registers]
    A --> F[CPU Scheduling Information]
    A --> G[Memory Management Information]
    A --> H[I/O Status Information]
    A --> I[Accounting Information]
```
