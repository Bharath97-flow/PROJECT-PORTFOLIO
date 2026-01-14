// main.h
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define EXTERNAL 1
#define INTERNAL 2
#define NO_COMMAND 3

/* job struct (type) */
typedef struct job {
    int id;
    pid_t pid;
    char command[256];
    char status[20];
} job_t;

/* Extern globals (declared here, defined exactly once in globals.c) */
extern int status;
extern int child_pid;
extern char last_command[256];
extern job_t jobs[100];
extern int job_count;
extern volatile sig_atomic_t stop_flag;

/* Prototypes */
void scan_input(char *prompt,char *input_string);
char *get_command(char *input_string);
int check_command(char *cmd);
void execute_internal_commands(char *input_string);
int echo(char *input,int status);
void sigint_handler(int signum);
void sigtstp_handler(int signum);

#endif /* MAIN_H */
