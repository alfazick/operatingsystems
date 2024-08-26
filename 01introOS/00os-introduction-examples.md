# Introduction to Operating Systems: Core Concepts and Python Examples

## Table of Contents
1. [Why Study Operating Systems?](#why-study-operating-systems)
2. [Introduction to OS: The Middleman](#introduction-to-os-the-middleman)
3. [Virtualizing Hardware](#virtualizing-hardware)
   - [CPU Virtualization](#cpu-virtualization)
   - [Memory Virtualization](#memory-virtualization)
4. [Concurrency: Managing Multithreaded Processes](#concurrency-managing-multithreaded-processes)
5. [Persistence: Managing Hard Memory](#persistence-managing-hard-memory)
6. [File System Management](#file-system-management)
7. [Security](#security)

## Why Study Operating Systems?

Operating Systems (OS) are crucial in computer science as they act as intermediaries between your programs and the hardware. Understanding OS concepts helps you write more efficient and robust software.

## Introduction to OS: The Middleman

The OS serves as a middleman between your program and the hardware. Here's a simple visualization:

```python
def your_program():
    print("Hello, World!")

def operating_system(program):
    print("OS: Starting program")
    program()
    print("OS: Program finished")

def hardware_execution():
    print("Hardware: Executing instructions")

# Simulating the flow: Your Program -> OS -> Hardware
operating_system(your_program)
hardware_execution()
```

## Virtualizing Hardware

### CPU Virtualization

The OS creates an illusion that each program has its own CPU, while in reality, it's sharing the CPU among multiple processes.

```python
import time

class Process:
    def __init__(self, pid, name, duration):
        self.pid = pid
        self.name = name
        self.duration = duration

class SimpleCPUScheduler:
    def __init__(self, time_slice=0.060):  # 60ms time slice
        self.processes = []
        self.time_slice = time_slice

    def add_process(self, process):
        self.processes.append(process)

    def run(self):
        while self.processes:
            current_process = self.processes.pop(0)
            print(f"Running process {current_process.name} for {self.time_slice*1000}ms")
            time.sleep(self.time_slice)
            
            current_process.duration -= self.time_slice
            if current_process.duration > 0:
                self.processes.append(current_process)
            else:
                print(f"Process {current_process.name} completed")
            
            print("System interrupt: switching process")

scheduler = SimpleCPUScheduler()
scheduler.add_process(Process(1, "P1", 0.15))
scheduler.add_process(Process(2, "P2", 0.10))
scheduler.run()
```

### Memory Virtualization

The OS provides each process with the illusion of having its own private memory space.

```python
class VirtualMemory:
    def __init__(self, size):
        self.memory = [0] * size
        self.page_table = {}

    def allocate(self, process_id, start, size):
        for i in range(size):
            self.page_table[(process_id, start + i)] = len(self.memory) + i
        self.memory.extend([0] * size)

    def write(self, process_id, address, value):
        if (process_id, address) in self.page_table:
            physical_address = self.page_table[(process_id, address)]
            self.memory[physical_address] = value
        else:
            print(f"Memory access violation: Process {process_id}, Address {address}")

    def read(self, process_id, address):
        if (process_id, address) in self.page_table:
            physical_address = self.page_table[(process_id, address)]
            return self.memory[physical_address]
        else:
            print(f"Memory access violation: Process {process_id}, Address {address}")
            return None

vm = VirtualMemory(100)
vm.allocate(1, 0, 10)
vm.write(1, 5, 42)
print(vm.read(1, 5))  # Output: 42
print(vm.read(1, 15))  # Output: Memory access violation: Process 1, Address 15
```

## Concurrency: Managing Multithreaded Processes

Concurrency allows multiple threads to execute in an interleaved fashion.

```python
import threading
import time

shared_resource = 0
lock = threading.Lock()

def increment():
    global shared_resource
    for _ in range(5):
        with lock:
            shared_resource += 1
            print(f"Incremented: {shared_resource}")
        time.sleep(0.1)

def decrement():
    global shared_resource
    for _ in range(5):
        with lock:
            shared_resource -= 1
            print(f"Decremented: {shared_resource}")
        time.sleep(0.1)

thread1 = threading.Thread(target=increment)
thread2 = threading.Thread(target=decrement)

thread1.start()
thread2.start()

thread1.join()
thread2.join()

print(f"Final value: {shared_resource}")
```

## Persistence: Managing Hard Memory

Persistence involves storing data on non-volatile storage devices.

```python
class SimpleFileSystem:
    def __init__(self):
        self.files = {}

    def write_file(self, filename, content):
        self.files[filename] = content
        print(f"File '{filename}' written to disk")

    def read_file(self, filename):
        if filename in self.files:
            print(f"Reading file '{filename}' from disk")
            return self.files[filename]
        else:
            print(f"File '{filename}' not found")
            return None

    def delete_file(self, filename):
        if filename in self.files:
            del self.files[filename]
            print(f"File '{filename}' deleted from disk")
        else:
            print(f"File '{filename}' not found")

fs = SimpleFileSystem()
fs.write_file("example.txt", "Hello, World!")
print(fs.read_file("example.txt"))
fs.delete_file("example.txt")
```


## File System Management

File systems organize and manage data on storage devices.

```python
class SimpleFileSystem:
    def __init__(self):
        self.files = {}
        self.directories = {"root": {}}

    def create_file(self, path, content=""):
        dir_path, filename = path.rsplit("/", 1)
        if dir_path in self.directories:
            self.files[path] = content
            self.directories[dir_path][filename] = path
            print(f"File created: {path}")
        else:
            print(f"Directory not found: {dir_path}")

    def read_file(self, path):
        if path in self.files:
            return self.files[path]
        else:
            print(f"File not found: {path}")
            return None

    def create_directory(self, path):
        self.directories[path] = {}
        print(f"Directory created: {path}")

    def list_directory(self, path):
        if path in self.directories:
            return list(self.directories[path].keys())
        else:
            print(f"Directory not found: {path}")
            return None

fs = SimpleFileSystem()
fs.create_directory("root/documents")
fs.create_file("root/documents/note.txt", "Hello, World!")
print(fs.read_file("root/documents/note.txt"))
print(fs.list_directory("root/documents"))
```

## Security

Security is a critical aspect of OS design, involving access control, encryption, etc.

```python
import hashlib

class SimpleSecuritySystem:
    def __init__(self):
        self.users = {}

    def create_user(self, username, password):
        hashed_password = self._hash_password(password)
        self.users[username] = hashed_password
        print(f"User '{username}' created")

    def authenticate(self, username, password):
        if username in self.users:
            hashed_password = self._hash_password(password)
            if self.users[username] == hashed_password:
                print(f"User '{username}' authenticated successfully")
                return True
        print("Authentication failed")
        return False

    def _hash_password(self, password):
        return hashlib.sha256(password.encode()).hexdigest()

security_system = SimpleSecuritySystem()
security_system.create_user("alice", "password123")
security_system.authenticate("alice", "password123")  # Successful
security_system.authenticate("alice", "wrong_password")  # Failed
```

