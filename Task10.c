#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

int my_cd(info_t *info) {
    char *current_dir, *new_dir, buffer[PATH_MAX];
    int chdir_result;

    current_dir = getcwd(buffer, PATH_MAX);
    if (!current_dir) {
        fprintf(stderr, "TODO: Handle getcwd failure here\n");
        return 1;
    }

    if (!info->argv[1] || strcmp(info->argv[1], "~") == 0) {
        new_dir = _getenv(info, "HOME=");
        if (!new_dir)
            chdir_result = chdir((new_dir = _getenv(info, "PWD=")) ? new_dir : "/");
        else
            chdir_result = chdir(new_dir);
    } else if (strcmp(info->argv[1], "-") == 0) {
        if (!_getenv(info, "OLDPWD=")) {
            printf("%s\n", current_dir);
            return 1;
        }
        printf("%s\n", _getenv(info, "OLDPWD="));
        chdir_result = chdir((new_dir = _getenv(info, "OLDPWD=")) ? new_dir : "/");
    } else {
        chdir_result = chdir(info->argv[1]);
    }

    if (chdir_result == -1) {
        perror("cd");
        return 1;
    }

    _setenv(info, "OLDPWD", current_dir);
    _setenv(info, "PWD", getcwd(buffer, PATH_MAX));

    return 0;
}
