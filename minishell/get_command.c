#include "main.h"
#include <fcntl.h>
#include <unistd.h>

char* get_command(char *input)
{
    static char cmd[20];
    int i = 0;

    if (!input) {
        cmd[0] = '\0';
        return cmd;
    }

    /* skip leading spaces */
    while (*input == ' ') input++;

    while (input[i] != '\0' && input[i] != ' ' && i < 19)
    {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = '\0';
    return cmd;
}

int check_command(char *cmd)
{
    char *builtins[] = {"printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
    for(int i=0; builtins[i]; i++)
    {
        if(cmd && !strcmp(cmd, builtins[i]))
        {
            return INTERNAL;
        }
    }

    int fd = open("external_commands.txt", O_RDONLY);
    if (fd < 0) {
        /* treat missing file as "no external list" */
        return NO_COMMAND;
    }

    char ch;
    int idx = 0;
    char input_cmd[30];
    while (read(fd, &ch, 1) > 0)
    {
        if (ch != '\n' && idx < (int)sizeof(input_cmd)-1)
        {
            input_cmd[idx++] = ch;
        }
        else
        {
            input_cmd[idx] = '\0';
            if (cmd && strcmp(cmd, input_cmd) == 0)
            {
                close(fd);
                return EXTERNAL;
            }
            /* reset */
            idx = 0;
            input_cmd[0] = '\0';
        }
    }

    close(fd);
    return NO_COMMAND;
}
