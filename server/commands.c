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
    user, pass, cwd, cdup, quit,
    dele, pwd, pasv, port, help,
    noop, retr, stor, list, NULL
};

void is_command(ftp_infos_t *ftp)
{
    int found = 0, i = 0;
    char *token;
    char *delim = " \n\r";
    token = strtok(ftp->buffer, delim);
    if (token != NULL) write(1, token, strlen(token));
    for (; token != NULL && commands[i] != NULL; i++) {
        if (strcmp(commands[i], token) == 0) {
            func_ptr[i](ftp);
            found = 1;
            break;
        }
    }
    if (found == 0) dprintf(ftp->tmp->socket, "500 Invalid command.\r\n");
    if (i != 8) dprintf(1, "\n");
}