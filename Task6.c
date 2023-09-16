#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>  
#include <unistd.h> 

ssize_t custom_input_buffer(info_t *info, char **buffer, size_t *length)
{
    ssize_t read_bytes = 0;
    size_t length_remaining = 0;

    if (!*length) /* If nothing left in the buffer, fill it */
    {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, custom_sigint_handler);

#if USE_GETLINE
        read_bytes = getline(buffer, &length_remaining, stdin);
#else
        read_bytes = custom_getline(info, buffer, &length_remaining);
#endif

        if (read_bytes > 0)
        {
            if ((*buffer)[read_bytes - 1] == '\n')
            {
                (*buffer)[read_bytes - 1] = '\0'; /* Remove trailing newline */
                read_bytes--;
            }
            info->linecount_flag = 1;
            custom_remove_comments(*buffer);
            custom_build_history_list(info, *buffer, info->histcount++);
            /* Check for a command chain with _strchr(*buffer, ';') here */
            {
                *length = read_bytes;
                info->cmd_buffer = buffer;
            }
        }
    }
    return read_bytes;
}

ssize_t custom_get_input(info_t *info)
{
    static char *buffer; /* The ';' command chain buffer */
    static size_t position_i, position_j, buffer_length;
    ssize_t read_bytes = 0;
    char **buffer_ptr = &(info->arguments), *p;

    _putchar(BUF_FLUSH);
    read_bytes = custom_input_buffer(info, &buffer, &buffer_length);
    if (read_bytes == -1) /* EOF */
        return -1;
    if (buffer_length) /* We have commands left in the chain buffer */
    {
        position_j = position_i; /* Initialize new iterator to current buffer position */
        p = buffer + position_i; /* Get pointer for return */

        custom_check_chain(info, buffer, &position_j, position_i, buffer_length);
        while (position_j < buffer_length) /* Iterate to semicolon or end */
        {
            if (custom_is_chain(info, buffer, &position_j))
                break;
            position_j++;
        }

        position_i = position_j + 1; /* Increment past nulled ';' */
        if (position_i >= buffer_length) /* Reached the end of the buffer? */
        {
            position_i = buffer_length = 0; /* Reset position and length */
            info->cmd_buffer_type = CMD_NORMAL;
        }

        *buffer_ptr = p; /* Pass back a pointer to the current command position */
        return custom_strlen(p); /* Return the length of the current command */
    }

    *buffer_ptr = buffer; /* Not a chain; pass back the buffer from custom_getline() */
    return read_bytes; /* Return the length of the buffer from custom_getline() */
}

ssize_t custom_read_buffer(info_t *info, char *buffer, size_t *i)
{
    ssize_t read_result = 0;

    if (*i)
        return 0;
    read_result = read(info->readfd, buffer, READ_BUF_SIZE);
    if (read_result >= 0)
        *i = read_result;
    return read_result;
}

int custom_getline(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUF_SIZE];
    static size_t position_i, buffer_length;
    size_t k;
    ssize_t read_result = 0, size = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        size = *length;
    if (position_i == buffer_length)
        position_i = buffer_length = 0;

    read_result = custom_read_buffer(info, buffer, &buffer_length);
    if (read_result == -1 || (read_result == 0 && buffer_length == 0))
        return -1;

    c = strchr(buffer + position_i, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;
    new_p = realloc(p, size ? size + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return p ? (free(p), -1) : -1;

    if (size)
        custom_strncat(new_p, buffer + position_i, k - position_i);
    else
        custom_strncpy(new_p, buffer + position_i, k - position_i + 1);

    size += k - position_i;
    position_i = k;
    p = new_p;

    if (length)
        *length = size;
    *ptr = p;
    return size;
}

void custom_sigint_handler(int sig_num)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}
