#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * main - super simple shell
 *
 * Return: Always 0.
 */
int main(void)
{
    char *cmd_line_input = NULL;
    size_t len = 0;
    ssize_t num_of_char;
    pid_t child_pid;
    int child_stat;
    char *argv[2];

    while (1)
    {
        printf("#cisfun$ ");
        num_of_char = getline(&cmd_line_input, &len, stdin);
        if (num_of_char == -1)
            break;
        
        /* Remove newline character */
        if (cmd_line_input[num_of_char - 1] == '\n')
            cmd_line_input[num_of_char - 1] = '\0';

        argv[0] = cmd_line_input;
        argv[1] = NULL;

        child_pid = fork();
        if (child_pid == -1)
        {
            perror("Error:");
            continue;
        }
        if (child_pid == 0)
        {
            if (execve(argv[0], argv, NULL) == -1)
            {
                perror("Error");
                exit(1);
            }
        }
        else
        {
            wait(&child_stat);
        }
    }

    free(cmd_line_input);
    return (0);
}