#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/*Global*/
	
	char *cmd_line = NULL;
	size_t length = 0;
	ssize_t read_line;

	/*Command line Print prompt*/
	printf("$ ");

	/*Read line input*/
	read_line = getline(&cmd_line, &length, stdin);
	if (read_line != -1)
	{
		printf ("%s", cmd_line);
	}
	
	return (0);
}

