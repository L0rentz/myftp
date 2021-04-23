/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** quit
*/

#include "ftp.h"

static int list_check_path(ftp_infos_t *ftp, char *token)
{
    DIR *dir;
    if (token != NULL) {
        char path[strlen(token) + strlen(ftp->tmp->path) + 1];
        path[0] = '\0', strcat(path, ftp->tmp->path), strcat(path, "/");
        strcat(path, token), dir = opendir(path);
        if (dir == NULL) {
            dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
            return (-1);
        }
        else {
            dprintf(ftp->tmp->socket, "150 File status okay; "
                "about to open data connection.\r\n");
            closedir(dir);
            return (1);
        }
    }
    dprintf(ftp->tmp->socket,
        "150 File status okay; about to open data connection.\r\n");
    return (0);
}

static void list_fork(ftp_infos_t *ftp, int check, char *token)
{
    pid_t pid = fork();
    if (pid != 0) return;
    FILE *fp = NULL;
    if (check > 0) {
        char command[strlen(ftp->tmp->path) + strlen(token) + 5];
        command[0] = '\0', strcat(command, "ls ");
        strcat(command, ftp->tmp->path), strcat(command, "/");
        strcat(command, token), fp = popen(command, "r");
    } else {
        char command[strlen(ftp->tmp->path) + 4];
        command[0] = '\0';
        strcat(command, "ls "), strcat(command, ftp->tmp->path);
        fp = popen(command, "r");
    }
    char buf[1024];
    while (fp != NULL && fgets(buf, 1024, fp) != NULL)
        dprintf(1, "%s", buf), dprintf(ftp->tmp->data_socket, "%s", buf);
    if (fp != NULL) pclose(fp);
    dprintf(ftp->tmp->socket, "226 Closing data connection.\r\n");
    longjmp(s_jumpBuffer, 0);
}

void list(ftp_infos_t *ftp)
{
    char *token, *delim = " \n\r";
    token = strtok(NULL, delim);
    if (token != NULL) dprintf(1, " %s", token);
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    } else if (ftp->tmp->mode == NONE) {
        dprintf(ftp->tmp->socket, "425 Use PORT or PASV first.\r\n");
        return;
    }
    int check = list_check_path(ftp, token);
    if (check < 0) return;
    list_fork(ftp, check, token);
    ftp->tmp->mode = NONE;
}