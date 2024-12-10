#include <stdio.h>
#include <stdlib.h>

void print_arguments(int **argv) {
    int *current_arg = *argv;

    // Iterate through the arguments until we find a NULL pointer
    while (*current_arg != NULL) {
        printf("Argument: %s\n", (char *)*current_arg);
        current_arg++;
    }
}

int main() {
    // Assuming argv is passed to main as a double pointer
    print_arguments((int **)&argv);

    return 0;
}

