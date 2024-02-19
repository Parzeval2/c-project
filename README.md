# TechShell: A Custom C Shell
## Overview
TechShell is a lightweight, custom shell implemented in C. It offers basic shell functionalities along with support for I/O redirection, handling quoted strings, and a dynamic display of the current working directory. Designed as part of a computer science course project, TechShell showcases the principles of Unix shell operation and process management.

## Features
- Command Execution: Execute standard Unix/Linux commands.
- I/O Redirection: Supports input (<), output (>), and append (>>) redirection.
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
- List files: ls -l
- Echo a string: echo "Hello, World!"
- Redirect output: ls -l > files.txt
- Append to a file: echo "New line" >> file.txt
## Known Issues and Limitations
- Currently does not support piping (|) between commands.
- Limited to basic error handling.
