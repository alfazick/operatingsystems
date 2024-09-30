# Bidirectional Inter-Process Communication with Pipes in Rust using libc

## Introduction

Inter-Process Communication (IPC) is a crucial concept in systems programming. One common method of IPC is using pipes. In this tutorial, we'll learn how to create bidirectional communication between a parent and child process using pipes in Rust, utilizing the libc crate for low-level system calls.

## Why Two Pipes?

Pipes in Unix-like systems are unidirectional. Data flows in one direction: from the write end to the read end. To achieve bidirectional communication, we need two pipes:

1. One pipe for Parent -> Child communication
2. Another pipe for Child -> Parent communication

## Step-by-Step Implementation

### Step 1: Setting up the Project

First, create a new Rust project:

```bash
cargo new bidirectional_ipc
cd bidirectional_ipc
```

Add the following dependencies to your `Cargo.toml`:

```toml
[dependencies]
libc = "0.2"
```

### Step 2: Implementing Bidirectional Communication

In your `src/main.rs`, use the following code:

```rust
use std::io::{Error, Result};
use std::os::unix::io::RawFd;
use std::time::Duration;
use std::thread;

const ITERATIONS: usize = 10;

fn main() -> Result<()> {
    let (parent_read, child_write) = create_pipe()?;
    let (child_read, parent_write) = create_pipe()?;

    match unsafe { libc::fork() } {
        -1 => return Err(Error::last_os_error()),
        0 => {
            // Child process
            unsafe {
                libc::close(parent_read);
                libc::close(parent_write);
            }
            child_process(child_read, child_write)?;
        }
        _ => {
            // Parent process
            unsafe {
                libc::close(child_read);
                libc::close(child_write);
            }
            parent_process(parent_read, parent_write)?;
        }
    }

    Ok(())
}

fn create_pipe() -> Result<(RawFd, RawFd)> {
    let mut fds: [libc::c_int; 2] = [-1; 2];
    if unsafe { libc::pipe(fds.as_mut_ptr()) } == -1 {
        return Err(Error::last_os_error());
    }
    Ok((fds[0], fds[1]))
}

fn parent_process(read_fd: RawFd, write_fd: RawFd) -> Result<()> {
    let mut buf = [0u8; 1024];
    for i in 0..ITERATIONS {
        let message = format!("Parent message {}", i);
        unsafe {
            libc::write(write_fd, message.as_ptr() as *const libc::c_void, message.len());
            let bytes_read = libc::read(read_fd, buf.as_mut_ptr() as *mut libc::c_void, buf.len());
            if bytes_read > 0 {
                let received = std::str::from_utf8(&buf[..bytes_read as usize]).unwrap();
                println!("Parent received: {}", received);
            }
        }
        thread::sleep(Duration::from_millis(100));
    }

    unsafe {
        libc::close(read_fd);
        libc::close(write_fd);
        let mut status = 0;
        libc::wait(&mut status);
    }

    Ok(())
}

fn child_process(read_fd: RawFd, write_fd: RawFd) -> Result<()> {
    let mut buf = [0u8; 1024];
    for _ in 0..ITERATIONS {
        unsafe {
            let bytes_read = libc::read(read_fd, buf.as_mut_ptr() as *mut libc::c_void, buf.len());
            if bytes_read > 0 {
                let received = std::str::from_utf8(&buf[..bytes_read as usize]).unwrap();
                println!("Child received: {}", received);
                let response = format!("Child echoing: {}", received);
                libc::write(write_fd, response.as_ptr() as *const libc::c_void, response.len());
            }
        }
    }

    unsafe {
        libc::close(read_fd);
        libc::close(write_fd);
    }

    Ok(())
}
```

### Step 3: Running the Program

Now you can run the program with `cargo run`. You should see output similar to this:

```
Child received: Parent message 0
Parent received: Child echoing: Parent message 0
Child received: Parent message 1
Parent received: Child echoing: Parent message 1
...
```

## Explanation

1. We create two pipes using the `create_pipe()` function, which wraps the libc `pipe()` system call.
2. We fork the process using `libc::fork()`, creating a child process.
3. In the parent process:
   - We close the unused pipe ends.
   - We write messages to the child in a loop.
   - We read responses from the child.
   - We wait for the child to finish using `libc::wait()`.
4. In the child process:
   - We close the unused pipe ends.
   - We read messages from the parent in a loop.
   - We write responses back to the parent.

This setup allows for full-duplex communication between the parent and child processes.

## Key Points

1. **Error Handling**: We use `Error::last_os_error()` to convert system errors into Rust's `std::io::Error`.
2. **Unsafe Code**: We use `unsafe` blocks when calling libc functions, as these are FFI calls and Rust can't guarantee their safety.
3. **File Descriptor Management**: We carefully close unused file descriptors to prevent resource leaks.
4. **Buffering**: We use fixed-size buffers for reading. In a real-world scenario, you might need to handle larger or variable-sized messages.
5. **Synchronization**: We use a simple loop with a sleep in the parent process. More complex applications might need more sophisticated synchronization mechanisms.

## Conclusion

Using two pipes for bidirectional communication between processes is a common and effective technique in systems programming. It allows for clear, safe, and efficient communication between parent and child processes.

Remember to always close unused pipe ends to prevent resource leaks and potential deadlocks. Also, be mindful of the buffer sizes when reading from pipes to avoid data loss.

This tutorial demonstrated a basic implementation using libc. In real-world scenarios, you might need to handle larger data transfers, implement more robust error checking, or set up more complex communication patterns.
