/*
* Name(s): Grant Cooper
* Date: 2/19/2023
* Description: Lightweight Shell Copy. From current Testing it appears to be able to mimic everything but autocomplete in the terminal
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

/**
 * @brief Struct that holds information about a shell command
 *
 * The struct contains the following fields:
 * command: The command name
 * argument_count: The number of arguments in the arguments array
 * arguments: An array of pointers to the arguments
 * input_redirection: The input redirection filename, or NULL if no redirection is specified
 * output_redirection: The output redirection filename, or NULL if no redirection is specified
 * append_out: A flag indicating whether the output should be appended to (1) or overwritten (0)
 */
struct ShellCommand {
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
    //error for if current working directory cannot be read
   if(getcwd(cwd, sizeof(cwd)) == NULL){
       perror("getcwd");
       return NULL;
   }
   //print working directory
    printf("%s$ ", cwd);

    //return user input
    char *prompt = NULL;
    size_t prompt_size = 0;
    getline(&prompt, &prompt_size, stdin);
    if(strcmp(prompt, "\n" ) == 0){
        return NULL;
    }
    if(strcmp(prompt, "exit\n") == 0){
        exit(0);
    }
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
    //inititalizes the structure and allocates memory
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
    //while loop to iterate through the input
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

        //special token cases
        if(token < next){
            //takes input redirection
            if(strcmp(token, "<")==0) {
                while (isspace((unsigned char) *next)) { //skip spaces
                    next++;
                }
                if (next != NULL) {
                    token = next;
                    while (*next && !isspace((unsigned char) *next)) {
                        next++;
                    }
                    if (*next) {
                        *next++ = '\0';
                    }
                    command.input_redirection = token;
                }
                else{
                    fprintf(stderr, "Invalid input redirection\n");
                }
            }
            //takes output redirection
            else if(strcmp(token, ">")==0)
            {
                while(isspace((unsigned char)*next)){ //skip spaces
                    next++;
                }
                if (next != NULL){
                    token = next;
                    while(*next &&!isspace((unsigned char)*next)){
                        next++;
                    }
                    if (*next) {
                        *next++ = '\0';
                    }
                    command.output_redirection=token;
                    command.append_out=0;
                }
                else{
                    perror("Invalid output redirection");
                }

            }
            //takes output redirection
            else if(strcmp(token, ">>")==0)
            {
                while(isspace((unsigned char)*next)){ //skip spaces
                    next++;
                }
                //ensures that it outputes to the full name of the file
                if(next != NULL) {
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
                //add token to arguments array
                command.arguments[command.argument_count++]=strdup(token);
            }
        }
    }
        if(command.argument_count>0)
        {
            //set command to first argument
            command.command=command.arguments[0];
        }

    return command;
}

/**
 * @brief Set up input and output redirection for a shell command
 *
 * @param command pointer to the shell command structure
 */
void SetupRedirect(struct ShellCommand *command) {
    if (command->input_redirection) {
        //open input redirection
        if(freopen(command->input_redirection, "r", stdin) == NULL){
            perror("Failed to open input redirection");
        }
    }
    if (command->output_redirection) {
        if (command->append_out) {
            //open append output redirection
            if(freopen(command->output_redirection, "a", stdout) == NULL){
                perror("Failed to redirect output");
            }
        } else {
            //open overwrite output redirection
            if(freopen(command->output_redirection, "w", stdout) ==NULL) {
                perror("Failed to redirect output");
            }
        }
    }
}
/**
 * @brief Set up input and output redirection for a shell command
 *
 * @param command pointer to the shell command structure
 */
void ExecuteCommand(struct ShellCommand command) {
    // Check if the command is 'cd'
    if (strcmp(command.command, "cd") == 0) {
        if (command.argument_count > 1) {
            // Change directory
            if (chdir(command.arguments[1]) != 0) {
                perror("chdir"); // Print error if chdir fails
            }
        } else {
            fprintf(stderr, "Usage: cd <directory>\n");
        }
        return; //no forking
    }
    //if command is not 'cd'
    //fork a child process
    pid_t pid = fork();
    if (pid == 0) {
        SetupRedirect(&command);
        char* arguments[command.argument_count + 1];
        //build array of arguments
        for (int i = 0; i < command.argument_count; i++) {
            arguments[i] = command.arguments[i];
        }
        //set arguments[command.argument_count] to NULL
        arguments[command.argument_count] = NULL;
        //execute command
        execvp(command.command, arguments);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) {
        //wait for child
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

        if (input == NULL) {
            continue;
        }
        // parse the shellCommand line
        shell_command = ParseCommandLine(input);
        // execute the shellCommand
        ExecuteCommand(shell_command);
    }
    exit(0);

}