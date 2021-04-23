/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** error
*/

#include "ftp.h"

int check_args(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-help") == 0) {
        dprintf(1, "USAGE: ./myftp port path\n"
            "       port is the port number on which the server socket "
            "listens\n       path is the path to the home directory for the "
            "Anonymous user\n");
        exit(0);
    }
    if (ac != 3) return (-1);
    int port = atoi(av[1]);
    if (port == 0 || port > 65535) return (-1);
    struct stat st = {0};
    if (stat(av[2], &st) == -1) return (-1);
    return (0);
}