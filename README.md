# Simple Shell

This project implements a simple interactive shell in C, allowing users to execute common Linux commands. It supports process management, foreground and background execution, and serial/parallel command execution using custom operators.

## Features

- **Basic Command Execution**: Supports the execution of Linux commands like `ls`, `cat`, `echo`, etc., through `execvp()`.
- **Built-in cd Command**: Change directories using `cd <directory>`.
- **Foreground and Background Execution**:
  - Default foreground execution.
  - Run commands in the background by appending `&`.
- **Serial and Parallel Command Execution**:
  - Commands separated by `&&` execute serially (one after the other).
  - Commands separated by `&&&` execute in parallel.
- **Signal Handling**: Custom handling of `SIGINT` (Ctrl+C) to terminate the current foreground process, but not the shell itself.
- **Exit Command**: Exit the shell with the `exit` command, which also terminates any running background processes.

## How to Run

1. Clone the repository:
    ```bash
    git clone <your-repo-url>
    ```

2. Compile the shell:
    ```bash
    gcc myshell.c -o myshell
    ```

3. Run the shell:
    ```bash
    ./myshell
    ```

## Usage

- **Run Commands**: Type any supported Linux command, e.g., `ls`, and press Enter.

- **Change Directory**: Use `cd` to change directories:
    ```bash
    cd <directory>
    ```

- **Run Commands in Background**: Add `&` at the end of a command to run it in the background:
    ```bash
    sleep 10 &
    ```

- **Serial Command Execution**: Use `&&` to run commands serially:
    ```bash
    command1 && command2
    ```

- **Parallel Command Execution**: Use `&&&` to run commands in parallel:
    ```bash
    command1 &&& command2
    ```

- **Exit the Shell**: Use `exit` to terminate the shell and all background processes:
    ```bash
    exit
    ```

## Example

```bash
$ ls
file1.txt  file2.txt
$ cd /home
$ ls && echo "Done"
file1.txt  file2.txt
Done
$ sleep 10 &
Shell: Background process PID 1234 finished
$ exit
