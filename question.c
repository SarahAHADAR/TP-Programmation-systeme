#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
//definition of the message
const char* WELCOME_MSG="Bienvenue dans le Shell ENSEA. \nPour quitter taper 'exit'.\n";
const char *ENSEASH="enseash % ";
#define CMD_MAX 1024
//writing of the message
int main(){
        char command_line[CMD_MAX]={0};
        ssize_t n;
        write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));//the massge is written on the standard output
//question 2c : main loop(REPL)
        while(1){
//question 1 and 2c: display Enseash  and await next command
                write(STDOUT_FILENO, ENSEASH, strlen(ENSEASH));
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
                        }
                }
        }
        return EXIT_SUCCESS;
}
