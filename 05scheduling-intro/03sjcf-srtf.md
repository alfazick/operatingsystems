# Complete Guide to Shortest Job to Completion First (SJCF) / Shortest Remaining Time First (SRTF) Scheduling

## 1. Introduction

Shortest Job to Completion First (SJCF), also known as Shortest Remaining Time First (SRTF), is a preemptive scheduling algorithm used in operating systems for process scheduling. It is an extension of the Shortest Job First (SJF) algorithm that allows for preemption, potentially interrupting running processes when a new process with a shorter remaining time arrives.

## 2. Algorithm Description

The SJCF/SRTF scheduling algorithm works as follows:

1. When a process arrives, it joins the ready queue.
2. The scheduler always selects the process with the shortest remaining time from all available processes.
3. The selected process runs until one of the following occurs:
   a) It completes execution
   b) A new process arrives with a shorter remaining time
4. If a new process with shorter remaining time arrives, the current process is preempted and added back to the ready queue.
5. Steps 2-4 repeat until all processes are completed.

The key feature of this algorithm is that it reevaluates the shortest remaining time at every unit of time or when a new process arrives, ensuring that the process with the least time to completion always gets priority.

## 3. Metrics

Several metrics are used to evaluate the performance of SJCF/SRTF scheduling:

1. Turnaround Time: The total time between a process's submission and its completion.
   Calculation: Turnaround Time = Completion Time - Arrival Time

2. Waiting Time: The total time a process spends in the ready queue.
   Calculation: Waiting Time = Turnaround Time - Burst Time

3. Response Time: The time from submission to the first time the process begins execution.

4. Throughput: Number of processes completed per unit time.

5. CPU Utilization: Percentage of time the CPU is actively executing processes.

## 4. Advantages and Disadvantages

Advantages:
1. Minimizes average waiting time and response time
2. Optimal for reducing average turnaround time
3. Adapts quickly to the arrival of short processes
4. Provides better overall system responsiveness

Disadvantages:
1. Potential starvation of longer processes
2. Requires continuous monitoring and recalculation of remaining times
3. Increased overhead due to frequent context switches
4. Difficult to implement in practice, as exact remaining times are often unknown
5. Can lead to increased average turnaround time for longer processes

## 5. Python Implementation

Here's a Python implementation of the SJCF/SRTF scheduling algorithm:

```python
import heapq
from copy import deepcopy

class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.completion_time = 0
        self.first_run_time = None
        self.turnaround_time = 0
        self.waiting_time = 0

    def __lt__(self, other):
        return self.remaining_time < other.remaining_time

def sjcf_scheduling(processes):
    n = len(processes)
    completed = 0
    current_time = 0
    processes_copy = deepcopy(processes)
    processes_copy.sort(key=lambda x: x.arrival_time)
    ready_queue = []
    current_process = None

    while completed < n:
        # Add newly arrived processes to the ready queue
        while processes_copy and processes_copy[0].arrival_time <= current_time:
            new_process = processes_copy.pop(0)
            heapq.heappush(ready_queue, new_process)
            if current_process and new_process.remaining_time < current_process.remaining_time:
                # Preempt the current process
                heapq.heappush(ready_queue, current_process)
                current_process = None

        if not current_process and ready_queue:
            current_process = heapq.heappop(ready_queue)
            if current_process.first_run_time is None:
                current_process.first_run_time = current_time

        if current_process:
            # Execute for one time unit
            current_process.remaining_time -= 1
            current_time += 1

            if current_process.remaining_time == 0:
                # Process completed
                completed += 1
                current_process.completion_time = current_time
                current_process.turnaround_time = current_process.completion_time - current_process.arrival_time
                current_process.waiting_time = current_process.turnaround_time - current_process.burst_time
                # Update the original process in the list
                for p in processes:
                    if p.pid == current_process.pid:
                        p.completion_time = current_process.completion_time
                        p.turnaround_time = current_process.turnaround_time
                        p.waiting_time = current_process.waiting_time
                        p.first_run_time = current_process.first_run_time
                        break
                current_process = None
        else:
            # No process to execute, just increment time
            current_time += 1

def print_results(processes):
    processes.sort(key=lambda x: x.pid)
    print("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse")
    for p in processes:
        response_time = p.first_run_time - p.arrival_time
        print(f"{p.pid}\t{p.arrival_time}\t{p.burst_time}\t{p.completion_time}\t\t{p.turnaround_time}\t\t{p.waiting_time}\t{response_time}")
    
    avg_turnaround = sum(p.turnaround_time for p in processes) / len(processes)
    avg_waiting = sum(p.waiting_time for p in processes) / len(processes)
    avg_response = sum(p.first_run_time - p.arrival_time for p in processes) / len(processes)
    print(f"\nAverage Turnaround Time: {avg_turnaround:.2f}")
    print(f"Average Waiting Time: {avg_waiting:.2f}")
    print(f"Average Response Time: {avg_response:.2f}")

if __name__ == "__main__":
    processes = [
        Process(1, 0, 8),
        Process(2, 1, 4),
        Process(3, 2, 2)
    ]

    sjcf_scheduling(processes)
    print_results(processes)
```

Key features of this implementation:
- Uses a min-heap to efficiently select the process with the shortest remaining time
- Handles preemption by reevaluating the shortest remaining time at each step
- Calculates and displays various performance metrics

## 6. Example Calculation

Let's walk through the execution of the example in the code:

Processes:
- P1: Arrival Time = 0, Burst Time = 8
- P2: Arrival Time = 1, Burst Time = 4
- P3: Arrival Time = 2, Burst Time = 2

Execution timeline:
- 0-1: P1 executes (remaining time: 7)
- 1-2: P2 preempts P1 (P2 remaining: 3, P1 remaining: 7)
- 2-4: P3 preempts P2 (P3 completes)
- 4-7: P2 resumes and completes
- 7-14: P1 resumes and completes

Results:
1. P1: 
   - Completion Time: 14
   - Turnaround Time: 14 - 0 = 14
   - Waiting Time: 14 - 8 = 6
   - Response Time: 0 - 0 = 0

2. P2:
   - Completion Time: 7
   - Turnaround Time: 7 - 1 = 6
   - Waiting Time: 6 - 4 = 2
   - Response Time: 1 - 1 = 0

3. P3:
   - Completion Time: 4
   - Turnaround Time: 4 - 2 = 2
   - Waiting Time: 2 - 2 = 0
   - Response Time: 2 - 2 = 0

Average Turnaround Time: (14 + 6 + 2) / 3 ≈ 7.33
Average Waiting Time: (6 + 2 + 0) / 3 ≈ 2.67
Average Response Time: (0 + 0 + 0) / 3 = 0

The Python script output should match these calculations:

```
PID     Arrival Burst   Completion      Turnaround      Waiting Response
1       0       8       14              14              6       0
2       1       4       7               6               2       0
3       2       2       4               2               0       0

Average Turnaround Time: 7.33
Average Waiting Time: 2.67
Average Response Time: 0.00
```

## 7. Conclusion

Shortest Job to Completion First (SJCF) or Shortest Remaining Time First (SRTF) scheduling is a highly responsive algorithm that minimizes average waiting and response times. It's particularly effective in environments where quick response to short tasks is crucial.

However, its preemptive nature can lead to increased overhead due to frequent context switches, and it may cause starvation for longer processes in systems with a constant influx of shorter jobs. Additionally, it's often impractical to implement in real systems because it requires accurate knowledge of process execution times, which are typically not known in advance.

Understanding SJCF/SRTF is valuable for grasping the trade-offs in scheduling algorithms, particularly the balance between responsiveness and fairness. While it may not be directly applicable in many real-world scenarios, the principles behind SJCF/SRTF inform more practical scheduling algorithms used in modern operating systems.

In practice, variants of this algorithm or hybrid approaches are often used to mitigate its drawbacks while still benefiting from its advantages in reducing average waiting and response times.

