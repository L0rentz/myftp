/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** noop
*/

#include "ftp.h"

void noop(ftp_infos_t *ftp)
{
    write(1, "NOOP\n", 5);
    write(ftp->tmp->socket, "200 Command okay.\r\n", 19);
}

void quit(ftp_infos_t *ftp)
{
    write(1, "QUIT\n", 5);
    if (ftp->tmp->logged == 1) {
        ftp->tmp->logged = 0;
        ftp->tmp->exist = 0;
        memset(ftp->tmp->pass, '\0', 65);
        memset(ftp->tmp->username, '\0', 65);
        ftp->tmp->quit = 1;
        write(ftp->tmp->socket,
            "221 Service closing control connection.\n"
            "    Logged out if appropriate.\r\n", 72);
    }
    else {
        write(ftp->tmp->socket, "530 Not logged in.\r\n", 20);
    }
}