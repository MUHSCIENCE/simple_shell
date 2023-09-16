#include <stdio.h>
#include <unistd.h>

/**
 * main - Entry point
 * @argc: The number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char *argv[])
{
    (void)argc;

    write(STDOUT_FILENO, argv[0], sizeof(argv[0]));
    write(STDOUT_FILENO, "\n", 1);

    return (0);
}

