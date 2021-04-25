/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** retr
*/

#include "ftp.h"

static int retr_check_path(ftp_infos_t *ftp, char *token)
{
    FILE *fp;
    if (token != NULL) {
        char path[strlen(token) + strlen(ftp->tmp->path) + 1];
        path[0] = '\0', strcat(path, ftp->tmp->path), strcat(path, "/");
        strcat(path, token);
        DIR* dir = opendir(path);
        if (dir != NULL) {
            closedir(dir);
            dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
            return (-1);
        } fp = fopen(path, "r");
        if (fp == NULL) {
            dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
            return (-1);
        } else dprintf(ftp->tmp->socket, "150 File status okay; "
                "about to open data connection.\r\n"), fclose(fp);
    } else {
        dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
        return (-1);
    } return (0);
}

static void retr_fork(ftp_infos_t *ftp, char *token)
{
    ftp->tmp->pid = fork();
    if (ftp->tmp->pid != 0) return;
    char path[strlen(token) + strlen(ftp->tmp->path) + 1];
    path[0] = '\0', strcat(path, ftp->tmp->path), strcat(path, "/");
    strcat(path, token);
    int fd = open(path, O_RDONLY), valread = 0;
    char buf[1025] = {0};
    while (fd != -1 && (valread = read(fd, buf, 1024)) > 0) {
        buf[valread] = '\0';
        write(1, buf, valread), write(ftp->tmp->data_socket, buf, valread);
    }
    if (fd != -1) close(fd);
    dprintf(ftp->tmp->socket, "\n226 Closing data connection.\r\n");
    longjmp(s_jumpBuffer, 0);
}

void retr(ftp_infos_t *ftp)
{
    char *token, *delim = " \n\r";
    token = strtok(NULL, delim);
    if (token != NULL) dprintf(1, " %s", token);
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    } else if (ftp->tmp->mode == NONE) {
        dprintf(ftp->tmp->socket, "421 Use PORT or PASV first.\r\n");
        return;
    }
    int check = retr_check_path(ftp, token);
    if (check < 0) return;
    ftp->tmp->transfer = 1;
    retr_fork(ftp, token);
    ftp->tmp->mode = NONE;
}