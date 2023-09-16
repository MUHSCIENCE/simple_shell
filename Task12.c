#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int is_chain_delimiter(info_t *info, char *buf, size_t *p)
{
    size_t n = *p;

    if (buf[n] == '|' && buf[n + 1] == '|')
    {
        buf[n] = 0;
        n++;
        info->cmd_buf_type = CMD_OR;
    }
    else if (buf[n] == '&' && buf[n + 1] == '&')
    {
        buf[n] = 0;
        n++;
        info->cmd_buf_type = CMD_AND;
    }
    else if (buf[n] == ';') /* found end of this command */
    {
        buf[n] = 0; /* replace semicolon with null */
        info->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *p = n;
    return (1);
}

void check_chain(info_t *info, char *buf, size_t *p, size_t m, size_t len)
{
    size_t n = *p;

    if (info->cmd_buf_type == CMD_AND)
    {
        if (info->status)
        {
            buf[m] = 0;
            n = len;
        }
    }
    if (info->cmd_buf_type == CMD_OR)
    {
        if (!info->status)
        {
            buf[m] = 0;
            n = len;
        }
    }

    *p = n;
}

int replace_alias(info_t *info)
{
    int m;
    list_t *node;
    char *n;

    for (m = 0; m < 10; m++)
    {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        n = _strchr(node->str, '=');
        if (!n)
            return (0);
        n = _strdup(n + 1);
        if (!n)
            return (0);
        info->argv[0] = n;
    }
    return (1);
}

int replace_vars(info_t *info)
{
    int m = 0;
    list_t *node;

    for (m = 0; info->argv[m]; m++)
    {
        if (info->argv[m][0] != '$' || !info->argv[m][1])
            continue;

        if (!_strcmp(info->argv[m], "$?"))
        {
            replace_string(&(info->argv[m]),
                           _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->argv[m], "$$"))
        {
            replace_string(&(info->argv[m]),
                           _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->argv[m][1], '=');
        if (node)
        {
            replace_string(&(info->argv[m]),
                           _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->argv[m], _strdup(""));
    }
    return (0);
}

int replace_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return (1);
}
