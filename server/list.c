/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** quit
*/

#include "ftp.h"

int list_check_path(ftp_infos_t *ftp, char *token)
{
    DIR *dir;
    if (token != NULL) {
        char path[strlen(token) + strlen(ftp->tmp->path) + 1];
        path[0] = '\0', strcat(path, ftp->tmp->path), strcat(path, "/");
        strcat(path, token), dir = opendir(path);
        if (dir == NULL) {
            write(ftp->tmp->socket, "550 Requested action not taken.\n    "
                "File unavailable (e.g., file not found, no access).\r\n", 89);
            return (-1);
        }
        else {
            write(ftp->tmp->socket, "150 File status okay; "
                "about to open data connection.\r\n", 54);
            closedir(dir);
            return (1);
        }
    }
    write(ftp->tmp->socket,
        "150 File status okay; about to open data connection.\r\n", 54);
    return (0);
}

void list_fork(ftp_infos_t *ftp, int check, char *token)
{
    pid_t pid = fork();
    if (pid == 0) {
        FILE *fp = NULL;
        if (check > 0) {
            char command[strlen(ftp->tmp->path) + strlen(token) + 5];
            command[0] = '\0';
            strcat(command, "ls "), strcat(command, ftp->tmp->path);
            strcat(command, "/"), strcat(command, token);
            fp = popen(command, "r");
        } else {
            char command[strlen(ftp->tmp->path) + 4];
            command[0] = '\0';
            strcat(command, "ls "), strcat(command, ftp->tmp->path);
            fp = popen(command, "r");
        }
        char buf[128];
        while (fp != NULL && fgets(buf, 128, fp) != NULL) {
            printf("%s", buf);
        }
        if (fp != NULL) pclose(fp);
    }
    else
        wait(NULL);
}

void list(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 0) {
        write(ftp->tmp->socket, "530 Not logged in.\r\n", 20);
        return;
    }
    char *token, *delim = " \n\r";
    token = strtok(NULL, delim);
    int check = list_check_path(ftp, token);
    if (check < 0) return;
    list_fork(ftp, check, token);
}