include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
//definition of the message
const char* WELCOME_MSG="Bienvenue dans le Shell ENSEA. \nPour quitter taper 'exit'.\n";
const char *ENSEASH="enseash % ";
const char *BYE_MSG="Bye Bye...\n"; //question 3: exit message
#define CMD_MAX 1024
// Question 4 we initialize the prompt to exit:0 to match the state after startup
#define PROMPT_MAX_SIZE 128
#define PROMPT_SIGNAL_FORMAT "enseash [sign:%d] %% "
#define PROMPT_EXIT_FORMAT "enseash [exit:%d] %% "

//Global variable for status 
int last_exit_status =0;
int last_signal = 0;
char prompt_buffer[PROMPT_MAX_SIZE];

void display_dynamic_prompt() {
        int len;
        if (last_signal!= 0){
                len=snprintf(prompt_buffer, sizeof(prompt_buffer), PROMPT_SIGNAL_FORMAT, last_signal);
        }
        else{
                len=snprintf(prompt_buffer, sizeof(prompt_buffer), PROMPT_EXIT_FORMAT,last_exit_status);
        }
        write(STDOUT_FILENO, prompt_buffer, len);
}
//writing of the message
int main(){
        char command_line[CMD_MAX]={0};
        ssize_t n;
        write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));//the massge is written on the standard output

//question 2c : main loop(REPL)
        while(1){
//question 1 and 2c: display Enseash  and await next command
                display_dynamic_prompt();
//question 2a read the entered command
                n=read(STDIN_FILENO, command_line, CMD_MAX);
                if (n==-1){
                        perror("read error");
                        continue;
}
                if(n==0){//end of file
                        break;
                }
//replace the final new line character '\n' with the null terminator '\0'
                if(n>0 && command_line[n-1]=='\n'){
                        command_line[n-1]='\0';
                }
                else{
                        command_line[n]='\0';
                }
//question 3 hamdle internal exit command
                if (strcmp(command_line, "exit")==0){
                        break;
                }
//question 2b execute a simple command
                pid_t pid=fork();
                int status;//to store the child's exit status
                if (pid==-1){//failed to create process
                        perror("fork error");
                        continue;
                }
                else if(pid==0){ //child process code
execlp(command_line, command_line, (char*)NULL);
// since there are no arguments, arg0 is the only argument passed
                        perror("execlp error:command not found or execution error");
                        exit(EXIT_FAILURE);
                }
                else{ // parent's process code
                        if (wait(&status)==-1){//wait for the created child process to terminate 
                                perror("wait error");
                                last_exit_status=127;
                                last_signal=0;

                        }
                        else{
                                if (WIFEXITED(status)){
                                        last_exit_status=WEXITSTATUS(status);
                                        last_signal = 0;
                                }
                                else if (WIFSIGNALED(status)){
                                        last_signal = WTERMSIG(status);
                                        last_exit_status=0;
                                }
                                else{
                                        last_exit_status=126;
                                        last_signal=0;}
                        }
                }
        }

        write(STDOUT_FILENO, BYE_MSG, strlen(BYE_MSG));
        return EXIT_SUCCESS;
}
