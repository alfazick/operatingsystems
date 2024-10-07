# Complete Guide to FIFO (First-In-First-Out) Scheduling

## 1. Introduction to FIFO Scheduling

FIFO (First-In-First-Out), also known as FCFS (First-Come-First-Served), is a simple scheduling algorithm used in operating systems and other computing contexts. It is one of the most basic scheduling algorithms and serves as a foundation for understanding more complex scheduling strategies.

## 2. General Algorithm

The FIFO scheduling algorithm works as follows:

1. Processes are executed strictly in the order they arrive in the ready queue.
2. The first process to arrive is the first one to be executed.
3. Once a process starts executing, it runs to completion without interruption (non-preemptive).
4. If a process is blocked (e.g., waiting for I/O), the next process in the queue is executed.

Pseudocode:
```
while (ready_queue is not empty)
    current_process = dequeue(ready_queue)
    execute(current_process)
    // Process runs until completion
```

Key characteristics:
- Non-preemptive: Once a process starts, it runs until it's finished or blocked.
- Simple to implement and understand.
- Fair in the sense that processes are executed in the order they arrive.

## 3. Metrics and Calculations

Several metrics are used to evaluate the performance of FIFO scheduling:

### 3.1 Turnaround Time
- Definition: The total time between a process's submission and its completion.
- Calculation: Turnaround Time = Completion Time - Arrival Time

### 3.2 Waiting Time
- Definition: The time a process spends waiting in the ready queue.
- Calculation: Waiting Time = Turnaround Time - Burst Time

### 3.3 Response Time
- Definition: The time from submission to the start of execution.
- In FIFO, this is the same as waiting time for each process.

### 3.4 Throughput
- Definition: Number of processes completed per unit time.
- Calculation: Throughput = Number of Processes / Total Execution Time

### 3.5 CPU Utilization
- Definition: Percentage of time the CPU is actively executing processes.
- In FIFO, this can be high if processes are CPU-bound, but may decrease with I/O-bound processes.

## 4. Example Calculation

Consider three processes P1, P2, and P3 with the following arrival and burst times:

- P1: Arrival Time = 0, Burst Time = 24 ms
- P2: Arrival Time = 1, Burst Time = 3 ms
- P3: Arrival Time = 2, Burst Time = 3 ms

Execution timeline:
- P1 starts at 0, completes at 24
- P2 starts at 24, completes at 27
- P3 starts at 27, completes at 30

Calculations:
1. P1: 
   - Turnaround Time = 24 - 0 = 24 ms
   - Waiting Time = 24 - 24 = 0 ms

2. P2:
   - Turnaround Time = 27 - 1 = 26 ms
   - Waiting Time = 26 - 3 = 23 ms

3. P3:
   - Turnaround Time = 30 - 2 = 28 ms
   - Waiting Time = 28 - 3 = 25 ms

Average Turnaround Time = (24 + 26 + 28) / 3 = 26 ms
Average Waiting Time = (0 + 23 + 25) / 3 = 16 ms

## 5. Advantages and Disadvantages

Advantages:
- Simple to understand and implement
- Fair in terms of arrival order
- No starvation (eventually, every process gets CPU time)

Disadvantages:
1. Convoy Effect: Short processes get stuck behind long ones, increasing average waiting time.
2. Poor for Interactive Systems: Long response times for short, interactive processes.
3. No Priority Handling: Cannot prioritize urgent or important tasks.
4. Inefficient CPU Utilization: CPU-bound processes can monopolize the CPU.
5. Not Optimal for Varied Process Types: Doesn't balance well between CPU-bound and I/O-bound processes.

## 6. Python Implementation

Here's a Python implementation of the FIFO scheduling algorithm:

```python
class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.completion_time = 0
        self.turnaround_time = 0
        self.waiting_time = 0

def fifo_scheduling(processes):
    # Sort processes by arrival time
    processes.sort(key=lambda x: x.arrival_time)
    
    current_time = 0
    for process in processes:
        # Update current time if there's a gap between processes
        if current_time < process.arrival_time:
            current_time = process.arrival_time
        
        # Calculate completion time
        process.completion_time = current_time + process.burst_time
        
        # Calculate turnaround time
        process.turnaround_time = process.completion_time - process.arrival_time
        
        # Calculate waiting time
        process.waiting_time = process.turnaround_time - process.burst_time
        
        # Update current time
        current_time = process.completion_time

def print_results(processes):
    print("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting")
    for p in processes:
        print(f"{p.pid}\t{p.arrival_time}\t{p.burst_time}\t{p.completion_time}\t\t{p.turnaround_time}\t\t{p.waiting_time}")
    
    avg_turnaround = sum(p.turnaround_time for p in processes) / len(processes)
    avg_waiting = sum(p.waiting_time for p in processes) / len(processes)
    print(f"\nAverage Turnaround Time: {avg_turnaround:.2f}")
    print(f"Average Waiting Time: {avg_waiting:.2f}")

# Example usage
processes = [
    Process(1, 0, 24),
    Process(2, 1, 3),
    Process(3, 2, 3)
]

fifo_scheduling(processes)
print_results(processes)
```

This implementation correctly handles process arrival times, calculates completion times, turnaround times, and waiting times for each process, and provides average metrics for the entire set of processes.

## 7. Conclusion

FIFO scheduling is a fundamental algorithm in process scheduling. While its simplicity makes it easy to implement and understand, its limitations make it less suitable for many real-world scenarios, especially in multi-user or time-sharing systems. More sophisticated algorithms like Shortest Job First (SJF), Round Robin, or Priority Scheduling are often preferred in practice. However, understanding FIFO is crucial as it forms the basis for comprehending more complex scheduling algorithms.

