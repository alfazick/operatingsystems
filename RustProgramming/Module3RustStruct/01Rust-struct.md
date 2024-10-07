# Struct

## Table of Contents
1. [Demonstrating How Structs Work](#1-demonstrating-how-structs-work)
2. [Explaining Impl Block, &self, and &mut self](#2-explaining-impl-block-self-and-mut-self)
3. [File Operations in Rust](#3-file-operations-in-rust)
4. [Executing OS Commands in Rust](#4-executing-os-commands-in-rust)
5. [Rust and Python Integration](#5-rust-and-python-integration)
   - [Rust Program (main.rs)](#rust-program-mainrs)
   - [Python Script (generate_dashboard.py)](#python-script-generate_dashboardpy)
6. [Integration Explanation](#6-integration-explanation)
7. [Instructions](#7-instructions)

## 1. Demonstrating How Structs Work

```rust
fn simple_struct() {
    struct Cat {
        name: String,
        age: u8,
    }

    let my_cat = Cat {
        name: String::from("Whiskers"),
        age: 3,
    };

    println!("My cat's name is {} and it is {} years old.", my_cat.name, my_cat.age);
}
```

## 2. Explaining Impl Block, &self, and &mut self

```rust
fn struct_impl_reference() {
    struct Cat {
        name: String,
        age: u8,
    }

    impl Cat {
        // A method that borrows self immutably
        fn introduce(&self) {
            println!("Meow! My name is {} and I am {} years old.", self.name, self.age);
        }

        // A method that borrows self mutably to update age
        fn have_birthday(&mut self) {
            self.age += 1;
            println!("{} is now {} years old. Happy Birthday!", self.name, self.age);
        }
    }

    let mut my_cat = Cat {
        name: String::from("Whiskers"),
        age: 3,
    };

    my_cat.introduce();
    my_cat.have_birthday();
}
```

## 3. File Operations in Rust

```rust
fn creating_and_writing_to_file() {
    use std::fs::File;
    use std::io::Write;

    let mut file = File::create("hello.txt").expect("Could not create file");
    writeln!(file, "Hello, file!").expect("Could not write to file");
}
```

## 4. Executing OS Commands in Rust

```rust
fn executing_os_commands_linux() {
    use std::process::Command;

    let output = Command::new("ls")
        .arg("-l")
        .output()
        .expect("Failed to execute command");

    println!("Command output: {}", String::from_utf8_lossy(&output.stdout));
}
```

## 5. Rust and Python Integration

### Rust Program (main.rs)

```rust
use std::fs::File;
use std::io::Write;
use std::process::Command;

struct LogSummary {
    info_count: u32,
    warn_count: u32,
    error_count: u32,
}

impl LogSummary {
    fn new() -> LogSummary {
        LogSummary {
            info_count: 0,
            warn_count: 0,
            error_count: 0,
        }
    }

    fn process_log(&mut self, log: &str) {
        if log.contains("INFO") {
            self.info_count += 1;
        } else if log.contains("WARN") {
            self.warn_count += 1;
        } else if log.contains("ERROR") {
            self.error_count += 1;
        }
    }

    fn save_to_file(&self) {
        let mut file = File::create("log_summary.txt").unwrap();
        writeln!(file, "INFO: {}", self.info_count).unwrap();
        writeln!(file, "WARN: {}", self.warn_count).unwrap();
        writeln!(file, "ERROR: {}", self.error_count).unwrap();
    }

    fn execute_python_script(&self) {
        Command::new("python")
            .arg("generate_dashboard.py")
            .status()
            .unwrap();
    }
}

fn main() {
    let logs = [
        "INFO: Operation successful",
        "ERROR: Failed to connect",
        "WARN: Low battery",
        "INFO: Data synced",
        "ERROR: Timeout occurred",
    ];

    let mut summary = LogSummary::new();
    for log in logs.iter() {
        summary.process_log(log);
    }
    summary.save_to_file();
    summary.execute_python_script();
}
```

### Python Script (generate_dashboard.py)

```python
import matplotlib.pyplot as plt

def generate_dashboard(summary_file_path='log_summary.txt'):
    counts = {'INFO': 0, 'WARN': 0, 'ERROR': 0}
    with open(summary_file_path, 'r') as file:
        for line in file:
            level, count = line.strip().split(": ")
            counts[level] = int(count)

    levels = list(counts.keys())
    occurrences = list(counts.values())

    plt.figure(figsize=(10, 6))
    plt.bar(levels, occurrences, color=['blue', 'orange', 'red'])
    plt.title('Log Level Occurrences')
    plt.xlabel('Log Level')
    plt.ylabel('Occurrences')
    plt.xticks(levels)
    plt.savefig('dashboard.png')
    plt.show()

if __name__ == "__main__":
    generate_dashboard()
```

## 6. Integration Explanation

This integration demonstrates how to combine Rust's efficient data processing capabilities with Python's flexible visualization tools:

- The Rust program processes log entries, summarizes them, and saves the summary to a file.
- It then executes a Python script using Rust's `Command` struct.
- The Python script reads the summary file and generates a matplotlib bar chart visualization.

This approach clearly separates the log processing and visualization tasks, showcasing how Rust can be used for efficient data processing and Python for flexible data visualization.

## 7. Instructions

1. Create a new Rust project:
   ```
   cargo new log_processor
   cd log_processor
   ```

2. Replace the contents of `src/main.rs` with the Rust code provided above.

3. Create a file named `generate_dashboard.py` in the same directory as your `Cargo.toml` file and paste the Python code provided above.

4. Ensure you have Python and matplotlib installed:
   ```
   pip install matplotlib
   ```

5. Run the Rust program:
   ```
   cargo run
   ```

This will process the log entries, create a summary file, and generate a bar chart visualization using Python and matplotlib.
