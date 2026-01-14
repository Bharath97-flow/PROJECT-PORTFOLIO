// globals.c
#include "main.h"

/* Real definitions — exactly one copy in the whole program */
int status = 0;
int child_pid = 0;
char last_command[256] = {0};
job_t jobs[100];
int job_count = 0;
volatile sig_atomic_t stop_flag = 0;
