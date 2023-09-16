#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        read = getline(&line, &len, stdin);

        if (read == -1)
        {
            free(line);
            exit(EXIT_SUCCESS);
        }

        /* Remove trailing newline */
        line[strcspn(line, "\n")] = 0;

        if (system(line) == -1)
        {
            write(STDERR_FILENO, "Error: command not found\n", 24);
        }
    }

    free(line);
    return (0);
}

