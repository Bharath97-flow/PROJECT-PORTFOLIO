#include"main.h"

void execute_internal_commands(char *input_string)
{
    if (strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    else if (strncmp(input_string, "cd", 2) == 0)
    {
        char path[1024];

        // If it's only "cd" with no path → go to HOME
        if (strlen(input_string) <= 3)
        {
            char *home = getenv("HOME");
            if (home)
                chdir(home);
            return;
        }

        // Extract path: skip "cd "
        strcpy(path, input_string + 3);

        if (chdir(path) != 0)
        {
            perror("cd");
        }
    }
    else if(strcmp(input_string,"pwd")==0)
    {
        system("pwd");
    }
}