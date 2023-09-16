#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **custom_get_environ(info_t *info)
{
    if (!info->environ || info->env_changed)
    {
        info->environ = custom_list_to_strings(info->environment);
        info->env_changed = 0;
    }

    return info->environ;
}

int custom_unset_environment_var(info_t *info, char *var)
{
    custom_list_t *node = info->environment;
    size_t i = 0;
    char *p;

    if (!node || !var)
        return 0;

    while (node)
    {
        p = custom_starts_with(node->str, var);
        if (p && *p == '=')
        {
            info->env_changed = custom_delete_node_at_index(&(info->environment), i);
            i = 0;
            node = info->environment;
            continue;
        }
        node = node->next;
        i++;
    }
    return info->env_changed;
}

int custom_set_environment_var(info_t *info, char *var, char *value)
{
    char *buf = NULL;
    custom_list_t *node;
    char *p;

    if (!var || !value)
        return 0;

    buf = malloc(custom_strlen(var) + custom_strlen(value) + 2);
    if (!buf)
        return 1;
    custom_strcpy(buf, var);
    custom_strcat(buf, "=");
    custom_strcat(buf, value);
    node = info->environment;
    while (node)
    {
        p = custom_starts_with(node->str, var);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            info->env_changed = 1;
            return 0;
        }
        node = node->next;
    }
    custom_add_node_end(&(info->environment), buf, 0);
    free(buf);
    info->env_changed = 1;
    return 0;
}
