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

int find_user(FILE *fp, ftp_infos_t *ftp)
{
    size_t len = 0, check = 0;
    ssize_t read = 0;
    char *delim = " \n\r", *token = NULL, *line = NULL;
    char *to_found = strtok(NULL, delim), *sha256 = NULL;
    if (to_found != NULL) sha256 = sha256_to_string(to_found);
    write(1, "USER", 4);
    if (sha256) write(1, " ", 1), write(1, sha256, 65);
    write(1, "\n", 1);
    while (to_found != NULL && (read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, delim);
        if (sha256 != NULL && strcmp(token, sha256) == 0) {
            snprintf(ftp->tmp->username, strlen(token)+1, "%s", token);
            token = strtok(NULL, delim), check = 1;
            if (token) snprintf(ftp->tmp->pass, strlen(token)+1, "%s", token);
            break;
        }
    }
    if (line) free(line);
    if (sha256) free(sha256);
    return (check);
}

void user(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 1) {
        write(ftp->tmp->socket, "230 User already logged in.\r\n", 30);
        return;
    }
    FILE *fp;
    fp = fopen("server/accounts.txt", "r");
    if (fp == NULL) {
        write(ftp->tmp->socket, "530 Connection not established.\r\n", 33);
        perror("Command USER: accounts.txt");
        return;
    }
    ftp->tmp->exist = find_user(fp, ftp);
    fclose(fp);
    if (ftp->tmp->exist == 1)
        write(ftp->tmp->socket, "331 User name okay, need password.\r\n", 36);
    else write(ftp->tmp->socket, "430 User name does not exist.\r\n", 31);
}

void pass_compare(char *to_found, ftp_infos_t *ftp)
{
    if (strcmp(ftp->tmp->pass, to_found) == 0) {
        ftp->tmp->logged = 1;
        write(ftp->tmp->socket, "230 User logged in, proceed.\r\n", 30);
    }
    else write(ftp->tmp->socket, "430 Password does not match.\r\n", 30);
    write(1, "PASS", 5);
    if (to_found[0] != '\0') {
        write(1, " ", 1);
        write(1, to_found, strlen(to_found));
    }
    write(1, "\n", 1);
}

void pass_cmd(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 1) {
        write(ftp->tmp->socket, "230 User already logged in.\r\n", 30);
        return;
    }
    if (ftp->tmp->exist == 0) {
        write(ftp->tmp->socket, "332 Need account for login.\r\n", 29);
        return;
    }
    char *delim = " \n\r";
    char *to_found = strtok(NULL, delim);
    if (to_found == NULL) pass_compare("", ftp);
    else {
        char *sha256 = sha256_to_string(to_found);
        pass_compare(sha256, ftp);
        free(sha256);
    }
}