# Complete Guide to Shortest Job First (SJF) Scheduling

## 1. Introduction to SJF Scheduling

Shortest Job First (SJF), also known as Shortest Job Next (SJN), is a scheduling algorithm that prioritizes the execution of the shortest jobs first. It aims to minimize the average waiting time for a given set of processes. SJF can be either preemptive or non-preemptive, with this guide focusing on the non-preemptive version.

## 2. General Algorithm

The SJF scheduling algorithm works as follows:

1. When a process arrives, it joins the ready queue.
2. The scheduler selects the process with the shortest burst time from all arrived processes in the ready queue.
3. The selected process runs to completion without interruption (non-preemptive).
4. After a process completes, the next shortest job among the arrived processes is selected for execution.

Pseudocode:
```
while (there are processes to execute)
    if (ready_queue is not empty)
        shortest_job = find_shortest_job(ready_queue)
        execute(shortest_job)
        remove(shortest_job, ready_queue)
    else
        wait_for_next_process_arrival()
```

Key characteristics:
- Can be preemptive (Shortest Remaining Time First) or non-preemptive.
- Aims to minimize average waiting time.
- Requires knowledge or estimation of process burst times.

## 3. Metrics and Calculations

Several metrics are used to evaluate the performance of SJF scheduling:

### 3.1 Turnaround Time
- Definition: The total time between a process's submission and its completion.
- Calculation: Turnaround Time = Completion Time - Arrival Time

### 3.2 Waiting Time
- Definition: The time a process spends waiting in the ready queue.
- Calculation: Waiting Time = Turnaround Time - Burst Time

### 3.3 Response Time
- Definition: The time from submission to the start of execution.
- In non-preemptive SJF, this is the same as waiting time for each process.

### 3.4 Throughput
- Definition: Number of processes completed per unit time.
- Calculation: Throughput = Number of Processes / Total Execution Time

### 3.5 CPU Utilization
- Definition: Percentage of time the CPU is actively executing processes.
- In SJF, this tends to be high as shorter jobs are prioritized.

## 4. Example Calculation

Consider three processes P1, P2, and P3 with the following arrival and burst times:

- P1: Arrival Time = 0, Burst Time = 6 ms
- P2: Arrival Time = 1, Burst Time = 4 ms
- P3: Arrival Time = 2, Burst Time = 2 ms

Execution timeline:
- P1 starts at 0, completes at 6 (no other process available at time 0)
- P3 starts at 6, completes at 8 (shortest among P2 and P3)
- P2 starts at 8, completes at 12

Calculations:
1. P1: 
   - Turnaround Time = 6 - 0 = 6 ms
   - Waiting Time = 6 - 6 = 0 ms

2. P2:
   - Turnaround Time = 12 - 1 = 11 ms
   - Waiting Time = 11 - 4 = 7 ms

3. P3:
   - Turnaround Time = 8 - 2 = 6 ms
   - Waiting Time = 6 - 2 = 4 ms

Average Turnaround Time = (6 + 11 + 6) / 3 ≈ 7.67 ms
Average Waiting Time = (0 + 7 + 4) / 3 ≈ 3.67 ms

## 5. Advantages and Disadvantages

Advantages:
- Minimizes average waiting time
- Increases system throughput by giving priority to shorter jobs
- Optimal for reducing average turnaround time
- Efficient for batch systems where job lengths are known

Disadvantages:
1. Potential starvation of longer processes
2. Requires knowledge of or ability to estimate process burst times
3. Not suitable for interactive systems where response time is critical
4. Can be complex to implement, especially in its preemptive form (SRTF)
5. May not be fair to all processes, as it prioritizes based on job length rather than arrival time

## 6. Python Implementation (Heap-based)

Here's an efficient Python implementation of the non-preemptive SJF scheduling algorithm using a heap:

```python
import heapq

class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.completion_time = 0
        self.turnaround_time = 0
        self.waiting_time = 0

    def __lt__(self, other):
        return self.burst_time < other.burst_time

def sjf_scheduling(processes):
    processes.sort(key=lambda x: x.arrival_time)
    n = len(processes)
    completed = 0
    current_time = 0
    heap = []
    i = 0

    while completed != n:
        # Add arrived processes to the heap
        while i < n and processes[i].arrival_time <= current_time:
            heapq.heappush(heap, processes[i])
            i += 1

        if not heap:
            current_time = processes[i].arrival_time
            continue

        # Get the shortest job from the heap
        shortest_job = heapq.heappop(heap)
        
        # Execute the shortest job
        current_time += shortest_job.burst_time
        shortest_job.completion_time = current_time
        shortest_job.turnaround_time = shortest_job.completion_time - shortest_job.arrival_time
        shortest_job.waiting_time = shortest_job.turnaround_time - shortest_job.burst_time
        
        completed += 1

def print_results(processes):
    processes.sort(key=lambda x: x.pid)
    print("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting")
    for p in processes:
        print(f"{p.pid}\t{p.arrival_time}\t{p.burst_time}\t{p.completion_time}\t\t{p.turnaround_time}\t\t{p.waiting_time}")
    
    avg_turnaround = sum(p.turnaround_time for p in processes) / len(processes)
    avg_waiting = sum(p.waiting_time for p in processes) / len(processes)
    print(f"\nAverage Turnaround Time: {avg_turnaround:.2f}")
    print(f"Average Waiting Time: {avg_waiting:.2f}")

# Example usage
if __name__ == "__main__":
    processes = [
        Process(1, 0, 6),
        Process(2, 1, 4),
        Process(3, 2, 2)
    ]

    sjf_scheduling(processes)
    print_results(processes)
```

This implementation uses a min-heap to efficiently select the shortest job among the arrived processes. The `heapq` module is used to maintain the heap of processes, ordered by their burst time.

Key features of this implementation:
- Processes are added to the heap as they arrive.
- The process with the shortest burst time is always at the top of the heap.
- If no processes are available, the algorithm jumps to the next arrival time.
- Time complexity for inserting or removing a process from the heap is O(log n), where n is the number of processes in the heap.

## 7. Conclusion

Shortest Job First scheduling is an efficient algorithm for minimizing average waiting time and turnaround time. While it's theoretically optimal for these metrics, its practical implementation faces challenges such as the need to know or estimate process burst times in advance. 

The heap-based implementation provided here offers an efficient way to manage processes and select the shortest job. This approach is particularly beneficial when dealing with a large number of processes, as it maintains good performance even as the number of processes increases.

SJF forms the basis for more advanced scheduling algorithms and is particularly useful in batch processing systems where job lengths are known. However, for interactive or real-time systems, other algorithms like Round Robin or Priority Scheduling might be more suitable. Understanding SJF and its implementation is crucial for grasping the trade-offs involved in process scheduling and for developing more sophisticated scheduling strategies.

