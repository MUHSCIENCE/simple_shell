#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int custom_print_env(info_t *info)
{
    custom_list_print_strings(info->environment);
    return (0);
}

char *custom_get_environment_var(info_t *info, const char *name)
{
    custom_list_t *node = info->environment;
    char *p;

    while (node)
    {
        p = custom_starts_with(node->str, name);
        if (p && *p)
            return (p);
        node = node->next;
    }
    return (NULL);
}

int custom_set_environment_var(info_t *info)
{
    if (info->argc != 3)
    {
        custom_eputs("Incorrect number of arguments\n");
        return (1);
    }
    if (custom_setenv(info, info->argv[1], info->argv[2])) // Replace with your setenv function if available
        return (0);
    return (1);
}

int custom_unset_environment_var(info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        custom_eputs("Too few arguments.\n");
        return (1);
    }
    for (i = 1; i <= info->argc; i++)
        custom_unsetenv(info, info->argv[i]); // Replace with your unsetenv function if available

    return (0);
}

int custom_populate_environment_list(info_t *info)
{
    custom_list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        custom_add_node_end(&node, environ[i], 0);
    info->environment = node;
    return (0);
}
