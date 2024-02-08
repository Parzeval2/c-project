/*
* Name(s): 
* Date: 
* Description: **Include what you were and were not able to handle!**
*
*
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/type.h>
#include <wait.h>

//Functions to implement:
char* CommandPrompt(){
    //display working directory
    char *cwd = getcwd(NULL, 0);
    printf("%s: ", cwd);

    //return user input

    scanf("%s", user_input);
    return user_input;

} // Display current working directory and return user input

struct ShellCommand ParseCommandLine(char* input); // Process the user input (As a shell command)

void ExecuteCommand(struct ShellCommand command); //Execute a shell command

int main() {
    char* input;
    struct ShellCommand command;

    // repeatedly prompt the user for input
    for (;;)
    {
        input = CommandPrompt();
        // parse the command line
        command = ParseCommandLine(input);
        // execute the command
        ExecuteCommand(command);
    }
    exit(0);

}