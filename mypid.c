#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t my_pid;
    pid_t par_pid;

    my_pid = getpid();
    par_pid = getppid();

    printf("%d is the child of %d.\n", my_pid, par_pid);

    return (0);
}
