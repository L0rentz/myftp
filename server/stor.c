/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** stor
*/

#include "ftp.h"

static char *stor_get_filename(ftp_infos_t *ftp, char *token)
{
    int i = strlen(token);
    i--;
    for (; token[i] != '\0'; i--)
        if (token[i] == '/' || i == 0) break;
    if (token[i] == '/') i++;
    char *filename = malloc(sizeof(char) *
        (strlen(ftp->tmp->path) + strlen(token + i) + 2));
    filename[0] = '\0';
    strcat(filename, ftp->tmp->path);
    strcat(filename, "/");
    strcat(filename, token + i);
    return (filename);
}

static void stor_fork(ftp_infos_t *ftp, char *token)
{
    ftp->tmp->pid = fork();
    if (ftp->tmp->pid != 0) return;
    ssize_t valread = 0;
    char buf[1025] = {0}, *filename = stor_get_filename(ftp, token);
    char buf_filename[strlen(filename)];
    strcpy(buf_filename, filename), free(filename);
    int fd = open(buf_filename, O_WRONLY | O_CREAT, 0644), ret = 0;
    if (fd == -1)
        dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
    else {
        struct pollfd fds;
        fds.events = POLLIN, fds.fd = ftp->tmp->data_socket;
        ret = poll(&fds, 1, 10000);
        while (ret > 0
        && (valread = read(ftp->tmp->data_socket, buf, 1024)) > 0) {
            buf[valread] = '\0';
            write(1, buf, valread), write(fd, buf, valread);
            if (valread < 1024) break;
        }
    } dprintf(ftp->tmp->socket, "\n226 Closing data connection.\r\n");
    longjmp(s_jumpBuffer, 0);
}

void stor(ftp_infos_t *ftp)
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
    } if (token == NULL) {
        dprintf(ftp->tmp->socket, "550 Requested action not taken.\r\n");
        return;
    }
    char tmp[PATH_MAX];
    getcwd(tmp, sizeof(tmp)), chdir(ftp->tmp->path);
    ftp->tmp->transfer = 1;
    dprintf(ftp->tmp->socket, "150 File status okay; "
        "about to open data connection.\r\n");
    stor_fork(ftp, token), chdir(tmp);
    ftp->tmp->mode = NONE;
}