#include <stdio.h>

char *my_strncpy(char *dest, const char *src, int max_chars)
{
    int index_dest = 0;
    char *destination = dest;

    for (index_dest = 0; index_dest < max_chars - 1 && src[index_dest] != '\0'; index_dest++)
    {
        dest[index_dest] = src[index_dest];
    }
    dest[index_dest] = '\0';

    return destination;
}

char *my_strncat(char *dest, const char *src, int max_chars)
{
    int dest_index = 0;
    int src_index = 0;
    char *destination = dest;

    while (dest[dest_index] != '\0')
    {
        // Find the end of dest
        dest_index++;
    }

    while (src[src_index] != '\0' && src_index < max_chars)
    {
        dest[dest_index] = src[src_index];
        dest_index++;
        src_index++;
    }
    dest[dest_index] = '\0';

    return destination;
}

char *my_strchr(const char *str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
        {
            return (char *)str;
        }
        str++;
    }

    return NULL;
}
