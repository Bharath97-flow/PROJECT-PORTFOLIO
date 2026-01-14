#include "main.h"

/* Do NOT define jobs/job_count/last_command/stop_flag here — only externs in main.h.
   Handlers must be minimal and async-signal-safe. */

void sigint_handler(int signum)
{
    (void)signum;
    if (child_pid > 0) {
        /* Forward SIGINT to foreground child */
        printf("\n");
        kill(child_pid, SIGINT);
    }
    /* Do NOT call printf/strcpy/malloc here */
}

void sigtstp_handler(int signum)
{
    (void)signum;
    if (child_pid > 0) {
        /* Send SIGSTOP to foreground child and notify main loop */
        kill(child_pid, SIGSTOP);
        stop_flag = 1;  /* volatile sig_atomic_t — async-safe */
    }
}
