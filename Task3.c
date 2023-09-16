#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h> 

int my_exit(info_t *info) {
    int exit_status;

    if (info->argv[1]) {
        exit_status = _strtoi(info->argv[1]);
        if (exit_status == -1) {
            info->status = 2;
            print_error(info, "Illegal number: ");
            _eputs(info->argv[1]);
            _eputchar('\n');
            return 1;
        }
        info->err_num = exit_status;
        return -2;
    }

    info->err_num = -1;
    return -2;
}

int my_cd(info_t *info) {
    char *current_dir, *new_dir, buffer[1024];
    int chdir_result;

    current_dir = getcwd(buffer, 1024);
    if (!current_dir)
        _puts("TODO: Handle getcwd failure here\n");

    if (!info->argv[1]) {
        new_dir = _getenv(info, "HOME=");
        if (!new_dir)
            chdir_result = chdir((new_dir = _getenv(info, "PWD=")) ? new_dir : "/");
        else
            chdir_result = chdir(new_dir);
    } else if (_strcmp(info->argv[1], "-") == 0) {
        if (!_getenv(info, "OLDPWD=")) {
            _puts(current_dir);
            _putchar('\n');
            return 1;
        }
        _puts(_getenv(info, "OLDPWD="));
        _putchar('\n');
        chdir_result = chdir((new_dir = _getenv(info, "OLDPWD=")) ? new_dir : "/");
    } else {
        chdir_result = chdir(info->argv[1]);
    }

    if (chdir_result == -1) {
        print_error(info, "can't cd to ");
        _eputs(info->argv[1]);
        _eputchar('\n');
    } else {
        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
        _setenv(info, "PWD", getcwd(buffer, 1024));
    }

    return 0;
}

int my_help(info_t *info) {
    (void)info; // Avoid unused parameter warning

    _puts("Help is not yet implemented\n");
    return 0;
}

  
