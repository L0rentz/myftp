/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** commands
*/

#include "ftp.h"

static const char *const commands[] = {
    "USER", "PASS", "CWD", "CDUP", "QUIT",
    "DELE", "PWD", "PASV", "PORT", "HELP",
    "NOOP", "RETR", "STOR", "LIST", NULL
};

static void (*func_ptr[])(ftp_infos_t *ftp) = {
    user, pass_cmd, noop, noop, quit,
    noop, noop, noop, noop, noop,
    noop, noop, noop, noop, NULL
};

void is_command(ftp_infos_t *ftp)
{
    int found = 0;
    char *token;
    char *delim = " \n\r";
    token = strtok(ftp->buffer, delim);
    if (token == NULL) return;
    for (int i = 0; commands[i] != NULL; i++) {
        if (strcmp(commands[i], token) == 0) {
            func_ptr[i](ftp);
            found = 1;
            break;
        }
    }
    if (found == 0) {
        write(1, ftp->buffer, strlen(ftp->buffer));
        write(1, "\n", 1);
    }
}