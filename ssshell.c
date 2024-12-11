#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

int main() {
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_length;

    while (1) {
        // Print prompt
        printf("#cisfun$ ");
        fflush(stdout);

        // Read input
        input_length = getline(&input, &input_size, stdin);

        // Handle end of file (Ctrl+D)
        if (input_length == -1) {
            printf("\nExiting shell...\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Exit condition
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            continue;
        }

        // Fork a child process
        pid_t pid = fork();

        if (pid < 0) {
            // Forking failed
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child process
            // Attempt to execute the command
            char *args[] = {input, NULL};
            execve(input, args, NULL);

            // If execve fails
            perror("execve");
            _exit(1);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);

            // Check if child process exited normally
            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    printf("Command failed with exit status %d\n", exit_status);
                }
            }
        }
    }

    // Free allocated memory
    free(input);

    return 0;
}
