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
#include <ctype.h>

struct ShellCommand{
    char *command;
    int argument_count;
    char **arguments;
    char *input_redirection;
    char *output_redirection;
    int append_out;
};
/**
 * @brief Display current working directory and return user input
 *
 * @return char* user input
 */
char* CommandPrompt(){
    //display working directory
    char cwd[1024];
   if(getcwd(cwd, sizeof(cwd)) == NULL){
       perror("getcwd");
       return NULL;
   }
    printf("%s: $", cwd);

    //return user input
    char *prompt = NULL;
    size_t prompt_size = 0;
    getline(&prompt, &prompt_size, stdin);
    return prompt;

} // Display current working directory and return user input

/**
 * @brief Process the user input (As a shell command)
 *
 * @param input user input
 * @return struct ShellCommand
 */
struct ShellCommand ParseCommandLine(char* input)
{
    struct ShellCommand command = {0};
    command.argument_count=0;
    command.arguments=malloc(10 *sizeof(char*));
    command.append_out=0;
    command.input_redirection=NULL;
    command.output_redirection=NULL;

    //commands and args
    char *token;
    char *next = input;
    char *end = input + strlen(input);

    while(next < end){
        while(isspace((unsigned char)*next)){ //skip spaces
            next++;
        }
        if(*next == '\"'){ //open quotes
            next++;
            token = next;
            while(*next && *next!= '\"'){ //closing quotes
                next++;
            }
        }
        else{ //regular token
            token = next;
            while(*next &&!isspace((unsigned char)*next) && *next!= '\"'){
                next++;
            }
        }

        if (*next){ //null terminator
            *next++ = '\0';
        }


        if(token < next){
            if(strcmp(token, "<")==0)
            {
                token = strtok(NULL, " \n");
                command.input_redirection=token;
            }
            else if(strcmp(token, ">")==0)
            {
                token = strtok(NULL, " \n");
                command.output_redirection=token;
                command.append_out=0;
            }
            else if(strcmp(token, ">>")==0)
            {
                while(isspace((unsigned char)*next)){ //skip spaces
                    next++;
                }
                if(next < end && *next!= '\0') {
                    token = next;
                    while(*next &&!isspace((unsigned char)*next)){
                        next++;
                    }
                    if (*next) {
                        *next++ = '\0';
                    }
                    command.output_redirection=strdup(token);
                    command.append_out=1;
                }
                else{
                    fprintf(stderr, "Error: Expected filename after '>>'\n");
                }
            }
            else
            {
                command.arguments[command.argument_count++]=strdup(token);
            }
        }
    }
        if(command.argument_count>0)
        {
            command.command=command.arguments[0];
        }

    return command;
}

void SetupRedirect(struct ShellCommand *command){
    if(command->input_redirection){
        freopen(command->input_redirection, "r", stdin);
    }
    if(command->output_redirection){
        if(command->append_out){
            freopen(command->output_redirection, "a", stdout);
        }
        else{
            freopen(command->output_redirection, "w", stdout);
        }
    }
}
void ExecuteCommand(struct ShellCommand command){
    pid_t pid = fork();
    if (pid == 0) {
        SetupRedirect(&command);
        char* arguments[command.argument_count + 1];
        for (int i = 0; i < command.argument_count; i++) {
            arguments[i] = command.arguments[i];
        }
        arguments[command.argument_count] = NULL;
        execvp(command.command, arguments);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    }
    else{
        perror("fork");
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
        if (strcmp(input, "exit") == 0) {
            break;
        }
        shell_command = ParseCommandLine(input);
        // execute the shellCommand
        ExecuteCommand(shell_command);
    }
    exit(0);

}