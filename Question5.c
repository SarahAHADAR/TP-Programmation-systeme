#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <time.h>      // NEW: for clock_gettime and struct timespec
#include <math.h>      // Optional: for math functions, though usually not needed here
//definition of the message
const char* WELCOME_MSG="Bienvenue dans le Shell ENSEA. \nPour quitter taper 'exit'.\n";
const char *ENSEASH="enseash % ";
const char *BYE_MSG="Bye Bye...\n"; //question 3: exit message
// Global variable for execution time
long long last_time_ms = 0; // Stores the execution time in milliseconds
#define CMD_MAX 1024
// Question 4 we initialize the prompt to exit:0 to match the state after startup
#define PROMPT_MAX_SIZE 128
// Prompt formats (Update to include time: |Tms)
#define PROMPT_SIGNAL_FORMAT "enseash [sign:%d|%lldms] %% "
#define PROMPT_EXIT_FORMAT "enseash [exit:%d|%lldms] %% "
// Note: %lld is used for long long
//Global variable for status 
int last_exit_status =0;
int last_signal = 0;
char prompt_buffer[PROMPT_MAX_SIZE];

void display_dynamic_prompt() {
    int len;

    if (last_signal != 0){
        // Format for termination by signal, including time
        len = snprintf(prompt_buffer, sizeof(prompt_buffer), PROMPT_SIGNAL_FORMAT, last_signal, last_time_ms); 
    }
    else{
        // Format for normal exit or exit with error, including time
        len = snprintf(prompt_buffer, sizeof(prompt_buffer), PROMPT_EXIT_FORMAT,last_exit_status, last_time_ms);
    }
    
    // Write the formatted prompt to STDOUT
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
                        struct timespec start_time, end_time;
                        clock_gettime(CLOCK_MONOTONIC,  &start_time);
                        if (wait(&status)==-1){//wait for the created child process to terminate 
                                perror("wait error");
                                last_exit_status=127;
                                last_signal=0;

                        }
                        else{
                                // Question 5: END time measurement
                                clock_gettime(CLOCK_MONOTONIC,  &end_time);
                                / Question 5: Calculate elapsed time in milliseconds (ms)
                                long long delta_sec = end_time.tv_sec - start_time.tv_sec;
                                long long delta_nsec = end_time.tv_nsec - start_time.tv_nsec;
                                // Total elapsed time in milliseconds
                                last_time_ms = delta_sec * 1000 + delta_nsec / 1000000; 
                                if (WIFEXITED(status)){
                                        last_exit_status=WEXITSTATUS(status);
                                        last_signal = 0;
                                }
        write(STDOUT_FILENO, BYE_MSG, strlen(BYE_MSG));
        return EXIT_SUCCESS;
}
