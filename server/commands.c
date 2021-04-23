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
    user, pass, cwd, noop, quit,
    noop, pwd, pasv, noop, help,
    noop, noop, noop, list, NULL
};

void is_command(ftp_infos_t *ftp)
{
    int found = 0;
    char *token;
    char *delim = " \n\r";
    token = strtok(ftp->buffer, delim);
    if (token != NULL) dprintf(1, token);
    for (int i = 0; token != NULL && commands[i] != NULL; i++) {
        if (strcmp(commands[i], token) == 0) {
            func_ptr[i](ftp);
            found = 1;
            break;
        }
    }
    if (found == 0) dprintf(ftp->tmp->socket, "500 Invalid command.\r\n");
    dprintf(1, "\n");
}