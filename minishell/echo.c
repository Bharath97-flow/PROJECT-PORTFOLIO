#include"main.h"


int echo(char *input, int status)
{
    //printf("[DEBUG] echo() CALLED with input = '%s'\n", input);

    // Skip "echo"
    char *arg = input + 4;

    // skip spaces
    while (*arg == ' ') arg++;

    if (!strcmp(arg, "$?")) {
        printf("%d\n", WEXITSTATUS(status));
    }
    else if (!strcmp(arg, "$$")) {
        printf("%d\n", getpid());
    }
    else if (!strcmp(arg, "$SHELL")) {
        printf("/bin/bash\n");
    }
    else {
        printf("%s\n", arg);
    }

    return 0;
}
