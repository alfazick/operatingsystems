# C Programming Lab: Linux Typical Workflow Example

This lab is fundamental to understanding how to interact effectively with a Linux-like shell.

## Pre-requisites

Before starting, make sure you have gcc installed on your Ubuntu system. If it's not installed, you can install it by running the following command:

```bash
sudo apt update
sudo apt install build-essential
```

The command `sudo apt install <package-name>` is commonly used on Debian-based Linux distributions like Ubuntu to install software packages. Let's break down what each part does:

- `sudo`: Stands for "superuser do." This command allows you to run other commands with administrative (superuser) permissions. Essentially, it lets you execute a command as the root user.
- `apt`: Short for "Advanced Package Tool." This is the package management system used by Debian and its derivatives like Ubuntu. It allows you to install, update, and remove software packages.
- `install`: This is the command that tells apt what you want to do—in this case, install a new package.
- `<package-name>`: This is the name of the package you want to install. Replace `<package-name>` with the actual package name.

So, when you run `sudo apt install build-essential`, you are asking the system to use administrative permissions to install the build-essential package. The build-essential package includes a bunch of development tools, like compilers and libraries, that are commonly used for building software on Ubuntu systems.

## Version 1: Simple Hello World Program

1. Create the File: Open your terminal and type the following command to create a new C file:

   ```bash
   touch hello.c
   ```

2. Edit the File: Open the file using a text editor like nano or vim. For this example, we'll use nano:

   ```bash
   nano hello.c
   ```

3. Add Code: Enter the following C code to print "Hello, world!":

   ```c
   #include <stdio.h>

   int main() {
       printf("Hello, world!\n");
       return 0;
   }
   ```

   Save the file and exit the editor.

4. Compile the Code: Compile the C code using gcc:

   ```bash
   gcc hello.c -o hello
   ```

5. Run the Program: After compilation, run the program:

   ```bash
   ./hello
   ```

   You should see `Hello, world!` printed on the terminal.

## Version 2: Accepts Name as Argument

Modify your `hello.c` to look like this:

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Hello, %s!\n", argv[1]);
    } else {
        printf("Hello, world!\n");
    }
    return 0;
}
```

Compile and run it as before. You can now run it with a name argument:

```bash
./hello Alice
```

This should print `Hello, Alice!`.

## Version 3: Optional Flag for Capitalization

Modify your `hello.c` to look like this:

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void capitalize(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int main(int argc, char *argv[]) {
    char greeting[100] = "Hello, world!";
    int should_capitalize = 0;

    if (argc > 1) {
        snprintf(greeting, sizeof(greeting), "Hello, %s!", argv[1]);
    }

    if (argc > 2 && strcmp(argv[2], "--capitalize") == 0) {
        should_capitalize = 1;
    }

    if (should_capitalize) {
        capitalize(greeting);
    }

    printf("%s\n", greeting);

    return 0;
}
```

Compile and run as before:

```bash
gcc hello.c -o hello
./hello Alice --capitalize
```

This should print `HELLO, ALICE!`.

## Most critical Point to observe:

Understanding this will make your life much easier:

In Unix-like operating systems such as Linux, the shell is essentially a user interface for running programs. Most of these programs are often small utilities written in C or other languages, each designed to do a specific task very well. (there are like 1000+ programs)

### Command Line Syntax

When you type a command in the shell, the typical syntax is:

```
command [arguments] [options/flags]
```

- Command: This is usually the name of the program you want to run.
- Arguments: These are positional parameters that the command needs to execute.
- Options/Flags: These are optional and are used to modify the behavior of the command. They usually start with a hyphen.

Our C example beautifully mirrors this structure:

- Command: When you run `./hello`, `./hello` is the command you're executing, similar to commands like `ls`.
- Arguments: `Alice` in `./hello Alice` is an argument, just like the filenames you would provide to a command like `cp`.
- Options/Flags: `--capitalize` in `./hello Alice --capitalize` is an optional flag that modifies how the command behaves.

Understanding this structure is fundamental to understanding how to interact effectively with a Unix-like shell, and your C program serves as a wonderful hands-on example to illustrate these concepts.

Basically, that's it - that's whole Linux programming in essence :)

I am joking, but it's not far from the truth.

### Basic Navigation Commands

Most useful commands are about navigation:


1. `pwd`: Print Working Directory
   - Shows the full pathname of the current working directory.
   ```bash
   pwd
   ```

2. `ls`: List
   - Lists all the files and directories in the current directory.
   ```bash
   ls
   ```

3. `cd`: Change Directory
   - Changes the current directory to the one specified in the arguments.
   ```bash
   cd /path/to/directory
   ```

4. `mkdir`: Make Directory
   - Creates a new directory.
   ```bash
   mkdir new_directory
   ```

5. `rmdir`: Remove Directory
   - Removes an empty directory.
   ```bash
   rmdir directory_name
   ```

### File Manipulation Commands

6. `touch`: Create File
   - Creates a new empty file.
   ```bash
   touch new_file.txt
   ```

7. `rm`: Remove File
   - Deletes a file. Use cautiously.
   ```bash
   rm file_name
   ```

8. `cp`: Copy
   - Copies files or directories.
   ```bash
   cp source destination
   ```

9. `mv`: Move
   - Moves files or directories, can also be used to rename files.
   ```bash
   mv old_name new_name
   ```

Most famous command, because joke says: go read manual page

10. `man`: Manual
    - Displays the user manual for the specified command.
    ```bash
    man command_name
    ```

These commands offer the basic functionalities you'd need to navigate and manipulate a Linux filesystem. They are the building blocks for many more complex operations and scripts.

