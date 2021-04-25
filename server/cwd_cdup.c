/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** cwd
*/

#include "ftp.h"

static int cwd_error_check(ftp_infos_t *ftp, char *token)
{
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return (-1);
    } if (token == NULL) {
        dprintf(ftp->tmp->socket, "550 Failed to change directory.\r\n");
        return (-1);
    }
    return (0);
}

static void cwd_response(ftp_infos_t *ftp, int ret, char *token, char *success)
{
    if (ret == -1 || token != NULL)
        dprintf(ftp->tmp->socket, "550 Failed to change directory.\r\n");
    else {
        char new_path[PATH_MAX] = {0};
        if (ftp->tmp->path != NULL)
            free(ftp->tmp->path), ftp->tmp->path = NULL;
        ftp->tmp->path = strdup(getcwd(new_path, sizeof(new_path)));
        dprintf(ftp->tmp->socket, "%s\r\n", success);
    }
}

void cwd(ftp_infos_t *ftp)
{
    char cwd[PATH_MAX] = {0};
    char *token = strtok(NULL, " \r\n");
    if (token != NULL) dprintf(1, " %s", token);
    if (cwd_error_check(ftp, token) == -1) return;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("\ngetcwd() error");
        dprintf(ftp->tmp->socket, "421 Service not available\r\n");
        return;
    }
    int ret = chdir(ftp->tmp->path);
    ret = chdir(token);
    token = strtok(NULL, " \r\n");
    cwd_response(ftp, ret, token, "250 Directory successfully changed.");
    chdir(cwd);
}

static void cdup_response(ftp_infos_t *ftp, int ret, char *success)
{
    if (ret == -1)
        dprintf(ftp->tmp->socket, "550 Failed to change directory.\r\n");
    else {
        char new_path[PATH_MAX] = {0};
        if (ftp->tmp->path != NULL)
            free(ftp->tmp->path), ftp->tmp->path = NULL;
        ftp->tmp->path = strdup(getcwd(new_path, sizeof(new_path)));
        dprintf(ftp->tmp->socket, "%s\r\n", success);
    }
}

void cdup(ftp_infos_t *ftp)
{
    char cwd[PATH_MAX] = {0};
    char *token = "..";
    if (cwd_error_check(ftp, token) == -1) return;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("\ngetcwd() error");
        dprintf(ftp->tmp->socket, "421 Service not available\r\n");
        return;
    }
    int ret = chdir(ftp->tmp->path);
    ret = chdir(token);
    cdup_response(ftp, ret, "250 Directory successfully changed.");
    chdir(cwd);
}