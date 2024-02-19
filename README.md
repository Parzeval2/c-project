# TechShell: A Custom C Shell
## Grant Cooper
## Overview
TechShell is a lightweight, custom shell implemented in C. It offers basic shell functionalities along with support for I/O redirection, handling quoted strings, and a dynamic display of the current working directory.

## Features
- Command Execution: Execute standard Unix/Linux commands.
- I/O Redirection: Supports input (`<`), output (`>`), and append (`>>`) redirection.
- Quoted String Handling: Correctly handles commands with quoted arguments, preserving spaces within quotes.
- Dynamic Prompt: Displays the current working directory in the prompt.
- Custom Command Parsing: Robust parsing mechanism to handle various command formats.

## Installation
Clone the repository and compile the source code using GCC:
````
git clone [repository-link]
cd techshell
gcc -o techshell techshell.c
````
## Usage
Run the shell from the terminal:
````
./techshell
````
Enter commands at the prompt as you would in any standard shell. Use exit to terminate the shell.

## Command Examples
- List files: `ls -l`
- Echo a string: `echo "Hello, World!"`
- Redirect output: `ls -l > files.txt`
- Append to a file: `echo "New line" >> file.txt`
## Known Issues and Limitations
- Currently does not support piping (`|`) between commands.
- Limited to basic error handling.


#Explanation
## 1. Prompt
- The shell displays the current working directory as part of the prompt.
- The user enters a command.
- `CommandPrompt()` function captures the input using `getline`, which allows for variable-length input.
- 
## 2. Parsing
- Input is passed to the `ParseCommandLine()` function.
- This function uses a custom parser to handle:
  - Regular commands and arguments.
  - Quoted strings, preserving spaces within quotes.
  - I/O redirection operators (`<`, `>`, `>>`).
- It separates the command from its arguments and identifies any redirection.
- 
## 3. Execution
- The `ExecuteCommand()` function handles the execution:
  - It forks a child process using `fork()`.
  - The child process sets up any specified I/O redirection using `freopen` in the `SetupRedirect()` function.
  - The command is executed in the child process using `execvp`.
  - The parent process waits for the child's completion using `waitpid`.

## 4. Redirection
- Redirection is managed by altering the standard input and output streams.
- For input redirection (`<`), the standard input (`stdin`) is redirected to read from a file.
- For output redirection (`>` or `>>`), the standard output (`stdout`) is redirected to write to a file, either overwriting it or appending to it.

## 5. Error Handling
- The program includes basic error handling for system calls and user inputs.
- Dynamic memory allocated during parsing and execution is carefully managed and freed to prevent leaks.
