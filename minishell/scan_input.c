#include "main.h"

/* parse_args retained here as before (uses static array) */
char **parse_args(char *input)
{
    static char *args[20];  /* supports up to 19 args */
    int i = 0;

    char *token = strtok(input, " ");
    while (token != NULL && i < 19)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;   /* VERY IMPORTANT */
    return args;
}

/* NOTE: NO globals defined here. Use externs from main.h/globals.c */

void scan_input(char *prompt, char *input_string)
{
    /* install handlers (main thread) */
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    while (1)
    {
        printf("%s", prompt);
        fflush(stdout);

        /* safer input read */
        if (fgets(input_string, 25, stdin) == NULL) {
            printf("\n");
            break;
        }
        /* strip newline */
        input_string[strcspn(input_string, "\n")] = '\0';

        /* skip empty */
        if (input_string[0] == '\0') {
            continue;
        }

        /* PS1 handling */
        if (strncmp(input_string, "PS1", 3) == 0)
        {
            if (input_string[3] == '=' && input_string[4] != ' ')
            {
                strncpy(prompt, &input_string[4], 24);
                prompt[24] = '\0';
                size_t len = strlen(prompt);
                if (len < 24) {
                    prompt[len] = '$';
                    prompt[len+1] = '\0';
                }
            }
            continue;
        }

        /* Save persistent copy BEFORE fork/execution */
        strncpy(last_command, input_string, sizeof(last_command)-1);
        last_command[sizeof(last_command)-1] = '\0';

        char *cmd = get_command(input_string);
        int ret = check_command(cmd);

        /* EXTERNAL: fork + exec */
        if (ret == EXTERNAL)
        {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            }

            if (pid == 0)
            {
                /* child: restore default handlers and exec */
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);

                char temp[100];
                strncpy(temp, input_string, sizeof(temp)-1);
                temp[sizeof(temp)-1] = '\0';
                char **args = parse_args(temp);
                execvp(args[0], args);
                perror("execvp failed");
                _exit(127);
            }
            else
            {
                /* parent */
                child_pid = pid;
                int s = 0;

                /* wait for that child, detect stopped with WUNTRACED */
                pid_t w = waitpid(pid, &s, WUNTRACED);
                if (w == -1) {
                    perror("waitpid");
                } else {
                    /* If child was stopped (Ctrl+Z) or stop_flag set by handler */
                    if (WIFSTOPPED(s) || stop_flag) {
                        if (job_count < 100) {
                            jobs[job_count].id = job_count + 1;
                            jobs[job_count].pid = pid;
                            strncpy(jobs[job_count].command, last_command, sizeof(jobs[job_count].command)-1);
                            jobs[job_count].command[sizeof(jobs[job_count].command)-1] = '\0';
                            strncpy(jobs[job_count].status, "Stopped", sizeof(jobs[job_count].status)-1);
                            jobs[job_count].status[sizeof(jobs[job_count].status)-1] = '\0';
                            printf("\n[%d]+  Stopped                 %s\n",
                                   jobs[job_count].id, jobs[job_count].command);
                            job_count++;
                        } else {
                            fprintf(stderr, "job table full\n");
                        }
                        stop_flag = 0;
                    }
                    /* If child exited normally: nothing to store */
                }

                child_pid = 0;
            }
        }
        /* INTERNAL builtins */
        else if (ret == INTERNAL)
        {
            execute_internal_commands(input_string);
        }
        /* jobs builtin (explicit check) */
        else if (strcmp(input_string, "jobs") == 0)
        {
            for (int i = 0; i < job_count; i++)
            {
                printf("[%d]   %s                 %s\n",
                       jobs[i].id,
                       jobs[i].status,
                       jobs[i].command);
            }
        }
        else
        {
            /* only treat as echo if the command is echo */
            if (strncmp(cmd, "echo", 4) == 0) {
                echo(input_string, status);
            } else {
                printf("%s: command not found\n", cmd);
            }
        }

        /* clear temp input buffer */
        input_string[0] = '\0';
    }
}
