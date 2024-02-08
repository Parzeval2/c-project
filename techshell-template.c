/*
* Name(s): 
* Date: 
* Description: **Include what you were and were not able to handle!**
*
*
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

struct ShellCommand{
    char *command;
    int argument_count;
    char **arguments;
};
char* CommandPrompt(){
    //display working directory
    char* cwd;
    cwd=(char*)get_current_dir_name();

    printf("%s: ", cwd);

    //return user input
    char *prompt;
    scanf("%s", prompt);
    return prompt;

} // Display current working directory and return user input

struct ShellCommand ParseCommandLine(char* input){
    struct ShellCommand command;
    command.argument_count=0;
    command.arguments=(char**)malloc(sizeof(char*)*100);

    //commands and args
    command.command = strtok(input, " ");
    while (command.command!= NULL) {
        command.arguments[command.argument_count] = command.command;
        command.argument_count++;
        command.command = strtok(NULL, " ");
    }

    //shell command struct

    return command;
} // Process the user input (As a shell command)

void ExecuteCommand(struct ShellCommand command){
    pid_t pid = fork();
    if (pid == 0) {
        char* arguments[command.argument_count + 1];
        arguments[0] = command.command;
        for (int i = 0; i < command.argument_count; i++) {
            arguments[i + 1] = command.arguments[i];
        }
        arguments[command.argument_count + 1] = NULL;
        execvp(command.command, arguments);
        exit(1);
    }
    else{
        int status;
        waitpid(pid, &status, 0);
    }
}//Execute a shell command

int main() {
    char* input;
    struct ShellCommand shell_command;

    // repeatedly prompt the user for input
    for (;;)
    {
        input = CommandPrompt();
        // parse the shellCommand line
        shell_command = ParseCommandLine(input);
        // execute the shellCommand
        ExecuteCommand(shell_command);
    }
    exit(0);

}