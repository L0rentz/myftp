/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** noop
*/

#include "ftp.h"

static int set_user_path(ftp_infos_t *ftp, int i, char *path)
{
    char *new_path = path;
    if (i == 0)
        new_path = ftp->arg_path;
    if (ftp->tmp->path != NULL) {
        free(ftp->tmp->path);
        ftp->tmp->path = NULL;
    }
    char cwd[PATH_MAX] = {0};
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcat(cwd, new_path);
    } else {
        perror("\ngetcwd() error");
        dprintf(ftp->tmp->socket, "421 Service not available\r\n");
        return (-1);
    }
    ftp->tmp->path = strdup(cwd);
    return (0);
}

static int find_user(FILE *fp, ftp_infos_t *ftp, char *delim, char *to_found)
{
    size_t len = 0, check = 0, i = 0;
    ssize_t read = 0;
    char *token = NULL, *line = NULL, *path = NULL, *sha256 = NULL;
    if (to_found != NULL) sha256 = sha256_to_string(to_found);
    if (sha256 != NULL) dprintf(1, " %s", sha256);
    while (to_found != NULL && (read = getline(&line, &len, fp)) != -1) {
        path = strtok(line, delim), token = strtok(NULL, delim);
        if (sha256 != NULL && strcmp(token, sha256) == 0) {
            if (set_user_path(ftp, i, path) == -1) break;
            snprintf(ftp->tmp->username, strlen(token)+1, "%s", token);
            token = strtok(NULL, delim), check = 1;
            if (token) snprintf(ftp->tmp->pass, strlen(token)+1, "%s", token);
            break;
        } i++;
    }
    if (line) free(line);
    if (sha256) free(sha256);
    return (check);
}

void user(ftp_infos_t *ftp)
{
    char *delim = " \n\r", *sha256 = NULL, *to_found = strtok(NULL, delim);
    if (to_found == NULL) {
        dprintf(ftp->tmp->socket, "501 Syntax error no arguments.\r\n");
        return;
    } else ftp->tmp->user_cmd = 1;
    if (ftp->tmp->logged == 1) {
        dprintf(ftp->tmp->socket, "230 User already logged in.\r\n");
        if (to_found != NULL) sha256 = sha256_to_string(to_found);
        if (sha256 != NULL) dprintf(1, " %s", sha256), free(sha256);
        return;
    }
    FILE *fp;
    fp = fopen("server/accounts.txt", "r");
    if (fp == NULL) {
        dprintf(ftp->tmp->socket, "550 Connection not established.\r\n");
        perror("Command USER: accounts.txt");
        return;
    }
    ftp->tmp->exist = find_user(fp, ftp, delim, to_found), fclose(fp);
    dprintf(ftp->tmp->socket, "331 Please specify the password.\r\n");
}

static void pass_compare(char *to_found, ftp_infos_t *ftp)
{
    if (ftp->tmp->exist == 1 && strcmp(ftp->tmp->pass, to_found) == 0
    && ftp->tmp->user_cmd == 1) {
        ftp->tmp->logged = 1;
        dprintf(ftp->tmp->socket, "230 User logged in, proceed.\r\n");
    } else if (ftp->tmp->user_cmd == 1) {
        ftp->tmp->user_cmd = 0;
        dprintf(ftp->tmp->socket, "430 Login incorrect.\r\n");
    }
}

void pass(ftp_infos_t *ftp)
{
    int check = 0;
    if (ftp->tmp->logged == 1) {
        dprintf(ftp->tmp->socket, "230 User already logged in.\r\n");
        check = 1;
    } else if (ftp->tmp->user_cmd == 0) {
        dprintf(ftp->tmp->socket, "332 Need account for login.\r\n");
        check = 1;
    }
    char *delim = " \n\r";
    char *to_found = strtok(NULL, delim);
    if (to_found == NULL && check == 0) pass_compare("", ftp);
    else if (to_found != NULL) {
        char *sha256 = sha256_to_string(to_found);
        if (check == 0) pass_compare(sha256, ftp);
        dprintf(1, " %s", sha256);
        free(sha256);
    }
}