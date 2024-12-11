#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_INPUT_LENGTH 1024

/**
 * handle_command - Executes a single word command
 * @command: The command to execute
 * Return: 0 on success, -1 on failure
 */
int handle_command(char *command) {
    pid_t pid;
    int status;
    char *args[2];
    extern char **environ;

    // Remove trailing newline
    command[strcspn(command, "\n")] = 0;

    // Skip empty commands
    if (strlen(command) == 0) {
        return 0;
    }

    // Prepare arguments for execve
    args[0] = command;
    args[1] = NULL;

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        // Forking failed
        perror("Error forking process");
        return -1;
    } else if (pid == 0) {
        // Child process
        if (execve(command, args, environ) == -1) {
            // If execve fails
            fprintf(stderr, "%s: command not found\n", command);
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 0;
}

/**
 * main - Entry point for simple shell
 * Return: Always 0
 */
int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Check if shell is in interactive mode
    int interactive = isatty(STDIN_FILENO);

    while (1) {
        // Display prompt only in interactive mode
        if (interactive) {
            printf("simple_shell$ ");
            fflush(stdout);
        }

        // Read input
        read = getline(&line, &len, stdin);

        // Handle EOF (Ctrl+D)
        if (read == -1) {
            printf("\n");
            break;
        }

        // Execute the command
        handle_command(line);
    }

    // Free allocated memory
    free(line);
    return 0;
}
