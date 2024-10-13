# Multi-Level Feedback Queue (MLFQ) Scheduler Implementation in Rust

## Introduction

In this assignment, you will implement key components of a Multi-Level Feedback Queue (MLFQ) scheduler in Rust. The MLFQ is an advanced scheduling algorithm that adapts to different types of processes, balancing responsiveness for short tasks and fairness for long-running processes.

## MLFQ Concept Clarification

1. Queue Structure:
   - The MLFQ consists of multiple queues, each representing a different priority level.
   - `queues[0]` is the highest priority queue, with subsequent queues having lower priorities.

2. Time Quanta:
   - Each queue has an associated time quantum, representing the maximum time a process can run in that queue before being preempted.
   - Time quanta typically increase for lower priority queues. For example: `[2, 4, 8]` means:
     - Processes in queue 0 get 2 time units
     - Processes in queue 1 get 4 time units
     - Processes in queue 2 get 8 time units

3. Process Priority:
   - A process's `priority` field represents its current queue index.
   - Valid priorities range from 0 to (num_levels - 1).
   - Priority 0 is the highest, corresponding to `queues[0]`.

4. Scheduling Behavior:
   - New processes typically start in the highest priority queue (priority 0).
   - Processes that use their full time quantum without completing move to a lower priority queue.
   - A periodic priority boost moves all processes back to the highest priority queue to prevent starvation.

## Setup

1. Create a new Rust project:
   ```
   cargo new mlfq_scheduler
   cd mlfq_scheduler
   ```
2. Replace the contents of `src/main.rs` with the following code:

```rust
mod mlfq;

fn main() {
    println!("MLFQ Scheduler Implementation");
    // You can add any demonstration code here if you wish
}
```

3. Create a new file `src/mlfq.rs` and copy the following starter code into it:

```rust
// src/mlfq.rs

#[derive(Clone)]
pub struct Process {
    pub id: u32,
    pub priority: usize,  // Represents the current queue index
    pub remaining_time: u32,
    pub total_executed_time: u32,
}

pub struct MLFQ {
    queues: Vec<Vec<Process>>,
    num_levels: usize,
    time_quanta: Vec<u32>,
    current_time: u32,
}

impl MLFQ {
    pub fn new(num_levels: usize, time_quanta: Vec<u32>) -> Self {
        MLFQ {
            queues: vec![Vec::new(); num_levels],
            num_levels,
            time_quanta,
            current_time: 0,
        }
    }

    // Exercise 1: Queue Management
    pub fn add_process(&mut self, process: Process) {
        // TODO: Implement this function
        // Add the process to the appropriate queue based on its priority
        // Ensure the priority is within the valid range (0 to num_levels - 1)
    }

    // Exercise 2: Process Execution
    pub fn execute_process(&mut self, queue_index: usize) {
        // TODO: Implement this function
        // Execute the process for its time quantum or until completion
        // Update remaining_time, total_executed_time, and current_time
        // Move the process to a lower priority queue if it doesn't complete
    }

    // Exercise 3: Priority Boost
    pub fn priority_boost(&mut self) {
        // TODO: Implement this function
        // Move all processes to the highest priority queue
        // Reset the priority of all processes to 0
    }

    // Simulate time passing and trigger a boost if needed
    pub fn update_time(&mut self, elapsed_time: u32) {
        self.current_time += elapsed_time;
        let boost_interval = 100;
        if self.current_time % boost_interval == 0 {
            self.priority_boost();
        }
    }
}

// Automated Test Cases
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add_process() {
        let mut mlfq = MLFQ::new(3, vec![2, 4, 8]);
        
        let process1 = Process { id: 1, priority: 0, remaining_time: 10, total_executed_time: 0 };
        let process2 = Process { id: 2, priority: 1, remaining_time: 5, total_executed_time: 0 };
        let process3 = Process { id: 3, priority: 5, remaining_time: 8, total_executed_time: 0 };

        mlfq.add_process(process1);
        mlfq.add_process(process2);
        mlfq.add_process(process3);

        assert_eq!(mlfq.queues[0].len(), 1);
        assert_eq!(mlfq.queues[1].len(), 1);
        assert_eq!(mlfq.queues[2].len(), 1);
    }

    #[test]
    fn test_execute_process() {
        let mut mlfq = MLFQ::new(3, vec![2, 4, 8]);
        mlfq.queues[0].push(Process { id: 1, priority: 0, remaining_time: 5, total_executed_time: 0 });

        mlfq.execute_process(0);

        assert_eq!(mlfq.queues[0].len(), 0);
        assert_eq!(mlfq.queues[1].len(), 1);
        assert_eq!(mlfq.queues[1][0].remaining_time, 3);
        assert_eq!(mlfq.queues[1][0].total_executed_time, 2);
    }

    #[test]
    fn test_priority_boost() {
        let mut mlfq = MLFQ::new(3, vec![2, 4, 8]);
        mlfq.queues[1].push(Process { id: 1, priority: 1, remaining_time: 5, total_executed_time: 3 });
        mlfq.queues[2].push(Process { id: 2, priority: 2, remaining_time: 3, total_executed_time: 7 });

        mlfq.update_time(100); // Should trigger priority boost

        assert_eq!(mlfq.queues[0].len(), 2);
        assert_eq!(mlfq.queues[1].len(), 0);
        assert_eq!(mlfq.queues[2].len(), 0);
    }

    #[test]
    fn test_boost_does_not_occur_prematurely() {
        let mut mlfq = MLFQ::new(3, vec![2, 4, 8]);
        mlfq.queues[1].push(Process { id: 1, priority: 1, remaining_time: 5, total_executed_time: 3 });
        
        mlfq.update_time(50); // No boost should happen

        assert_eq!(mlfq.queues[1].len(), 1);
        assert_eq!(mlfq.queues[0].len(), 0);
    }
}
```

## Implementation Tasks

Implement the following functions in the `MLFQ` struct:

1. `add_process(&mut self, process: Process)`
   - Add the given process to the appropriate queue based on its priority.
   - Ensure the process's priority is within the valid range (0 to num_levels - 1).
   - If the priority is out of range, place it in the lowest priority queue.

2. `execute_process(&mut self, queue_index: usize)`
   - Simulate the execution of a process from the specified queue.
   - Use the appropriate time quantum for the queue.
   - Update the process's remaining time and total executed time.
   - Move the process to a lower priority queue if it's not completed and has used its full time quantum.
   - Remove the process if it's completed.
   - Update the MLFQ's current time.

3. `priority_boost(&mut self)`
   - Move all processes from lower priority queues to the highest priority queue.
   - Reset the priority of all moved processes to 0 (highest priority).

## Guidelines for Implementation

1. Use Rust's ownership and borrowing rules correctly.
2. Handle edge cases, such as empty queues or processes with zero remaining time.
3. Ensure that the time quantum for each queue is respected during process execution.
4. Use Rust's vector methods efficiently for queue operations.
5. Add comments to explain your logic, especially for complex parts of the implementation.

## Testing

The provided code includes a set of unit tests. Implement your functions to pass these tests. To run the tests, use the command:
```
cargo test
```

Feel free to add more tests to thoroughly validate your implementation.

## Submission Requirements

1. Implement the required functions in `src/mlfq.rs`.
2. Ensure all provided tests pass.
3. Add comments explaining your implementation logic.
4. Submit your completed `mlfq.rs` file.

## Grading Criteria

Your implementation will be graded on:
1. Correctness: All provided tests should pass.
2. Code quality: Clear, well-organized, and efficiently written code.
3. Adherence to Rust best practices and idioms.
4. Handling of edge cases and error conditions.
5. Quality of added comments and any additional tests.
6. Correct implementation of MLFQ concepts, including proper queue management and priority adjustments.

Good luck with your implementation!

