#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
	// Global
    pid_t pid;
    int status;
    
    // Create 5 child processes sequentially
    for (int i = 0; i < 5; i++) {
        // Fork a new child process
        pid = fork();
        
        if (pid < 0) {
            // Fork failed
            fprintf(stderr, "Fork failed on iteration %d\n", i);
            exit(1);
        }
        
        if (pid == 0) {
            // Child process
            printf("Child process %d executing ls -l /tmp\n", getpid());
            
            // Execute ls -l /tmp
            // Arguments must be NULL-terminated
            char *args[] = {"ls", "-l", "/tmp", NULL};
            execve("/bin/ls", args, NULL);
            
            // If execve fails, we'll reach this point
            perror("execve failed");
            exit(1);
        }
        
        // Parent process waits for the child to complete
        waitpid(pid, &status, 0);
        
        // Check if child exited normally
        if (WIFEXITED(status)) {
            printf("Child process %d completed with exit status %d\n", 
                   pid, WEXITSTATUS(status));
        }
    }
    
    return 0;
}

