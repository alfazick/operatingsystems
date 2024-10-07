# Complete Guide to Round Robin Scheduling

## 1. Introduction

Round Robin (RR) is a CPU scheduling algorithm widely used in operating systems, particularly in time-sharing and multitasking environments. It is designed to provide fair execution time to all processes by assigning a fixed time quantum or time slice to each process in a circular order. Round Robin is preemptive and aims to balance responsiveness with throughput.

## 2. Algorithm Description

The Round Robin scheduling algorithm works as follows:

1. Processes are kept in a circular queue (ready queue).
2. The CPU is assigned to a process for a fixed time quantum (typically 10-100 milliseconds).
3. After the time quantum expires or if the process completes or gets blocked:
   a) If completed or blocked, remove from queue.
   b) If still running, place at the back of the queue.
4. The CPU is then assigned to the next process in the queue.
5. Steps 2-4 repeat until all processes are completed.

Key characteristics:
- Preemptive: Processes are forcibly switched after their time quantum expires.
- Fair: Each process gets an equal share of CPU time.
- Suitable for time-sharing systems.
- Performance depends heavily on the size of the time quantum.

## 3. Metrics

Several metrics are used to evaluate the performance of Round Robin scheduling:

1. Turnaround Time: The total time between a process's submission and its completion.
   Calculation: Turnaround Time = Completion Time - Arrival Time

2. Waiting Time: The total time a process spends waiting in the ready queue.
   Calculation: Waiting Time = Turnaround Time - Burst Time

3. Response Time: The time from submission to the first time the process begins execution.

4. Throughput: Number of processes completed per unit time.

5. CPU Utilization: Percentage of time the CPU is actively executing processes.

## 4. Advantages and Disadvantages

Advantages:
1. Fair allocation of CPU time to all processes
2. Good for time-sharing environments
3. Responsive, as each process gets regular CPU time
4. Prevents starvation of processes
5. Good for processes with similar burst times

Disadvantages:
1. Higher average turnaround time compared to SJF
2. Performance is sensitive to time quantum selection
3. If time quantum is too small, it leads to excessive context switching
4. If time quantum is too large, it degenerates to FCFS
5. Does not prioritize urgent or high-priority tasks

## 5. Python Implementation

Here's a corrected Python implementation of the Round Robin scheduling algorithm:

```python
from collections import deque
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

def round_robin_scheduling(processes, time_quantum):
    n = len(processes)
    ready_queue = deque()
    current_time = 0
    completed = 0
    
    # Create a deep copy of processes to work with
    process_copy = deepcopy(processes)
    process_copy.sort(key=lambda x: x.arrival_time)
    
    while completed < n:
        # Add newly arrived processes to the ready queue
        while process_copy and process_copy[0].arrival_time <= current_time:
            ready_queue.append(process_copy.pop(0))
        
        if ready_queue:
            current_process = ready_queue.popleft()
            
            if current_process.first_run_time is None:
                current_process.first_run_time = current_time
            
            if current_process.remaining_time <= time_quantum:
                # Process will complete in this time quantum
                current_time += current_process.remaining_time
                current_process.completion_time = current_time
                current_process.remaining_time = 0
                completed += 1
            else:
                # Process will not complete in this time quantum
                current_time += time_quantum
                current_process.remaining_time -= time_quantum
                ready_queue.append(current_process)
            
            # Calculate metrics if process is completed
            if current_process.remaining_time == 0:
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
        else:
            # No process in ready queue, move time to next arrival
            if process_copy:
                current_time = process_copy[0].arrival_time
            else:
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
        Process(1, 0, 10),
        Process(2, 1, 5),
        Process(3, 3, 8)
    ]
    time_quantum = 2

    round_robin_scheduling(processes, time_quantum)
    print_results(processes)
```

Key features of this implementation:
- Uses a deque (double-ended queue) for efficient queue operations
- Creates a deep copy of the process list to preserve original data
- Handles process arrivals and executions based on the time quantum
- Updates original processes with their final metrics
- Calculates and displays various performance metrics

## 6. Example Calculation

Let's walk through the execution of the example in the code:

Processes:
- P1: Arrival Time = 0, Burst Time = 10
- P2: Arrival Time = 1, Burst Time = 5
- P3: Arrival Time = 3, Burst Time = 8

Time Quantum = 2

Execution timeline:
- 0-2: P1 executes (remaining time: 8)
- 2-4: P2 executes (remaining time: 3)
- 4-6: P3 executes (remaining time: 6)
- 6-8: P1 executes (remaining time: 6)
- 8-10: P2 executes (completes)
- 10-12: P3 executes (remaining time: 4)
- 12-14: P1 executes (remaining time: 4)
- 14-16: P3 executes (remaining time: 2)
- 16-18: P1 executes (remaining time: 2)
- 18-20: P3 executes (completes)
- 20-22: P1 executes (completes)

Results:
1. P1: 
   - Completion Time: 22
   - Turnaround Time: 22 - 0 = 22
   - Waiting Time: 22 - 10 = 12
   - Response Time: 0 - 0 = 0

2. P2:
   - Completion Time: 10
   - Turnaround Time: 10 - 1 = 9
   - Waiting Time: 9 - 5 = 4
   - Response Time: 2 - 1 = 1

3. P3:
   - Completion Time: 20
   - Turnaround Time: 20 - 3 = 17
   - Waiting Time: 17 - 8 = 9
   - Response Time: 4 - 3 = 1

Average Turnaround Time: (22 + 9 + 17) / 3 ≈ 16.00
Average Waiting Time: (12 + 4 + 9) / 3 ≈ 8.33
Average Response Time: (0 + 1 + 1) / 3 ≈ 0.67

The Python script output should match these calculations:

```
PID     Arrival Burst   Completion      Turnaround      Waiting Response
1       0       10      22              22              12      0
2       1       5       10              9               4       1
3       3       8       20              17              9       1

Average Turnaround Time: 16.00
Average Waiting Time: 8.33
Average Response Time: 0.67
```

## 7. Conclusion

Round Robin scheduling is a widely used algorithm in time-sharing systems due to its fairness and ability to provide good response times for all processes. It prevents any single process from monopolizing the CPU, making it suitable for environments where equitable distribution of CPU time is crucial.

The performance of Round Robin heavily depends on the choice of time quantum. A smaller time quantum leads to more frequent context switches but better responsiveness, while a larger time quantum reduces context switching overhead but may lead to poor response times for short processes.

While Round Robin may not provide the optimal turnaround or waiting times compared to algorithms like Shortest Job First, its simplicity, fairness, and prevention of starvation make it a popular choice in many real-world systems. It forms the basis for more advanced scheduling algorithms and is often used in combination with other techniques to balance fairness, responsiveness, and overall system performance.

In practice, variations of Round Robin, such as weighted Round Robin or multi-level feedback queues, are often employed to address some of its limitations and provide better performance for diverse workloads in modern operating systems.

