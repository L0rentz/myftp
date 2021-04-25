/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** noop
*/

#include "ftp.h"

static const char help_buf[] =
    "214 USER <SP> <username> <CRLF>   : Specify user for authentication\n"
    "    PASS <SP> <password> <CRLF>   : Specify password for authentication\n"
    "    CWD  <SP> <pathname> <CRLF>   : Change working directory\n"
    "    CDUP <CRLF>                   : Change working directory to parent "
    "directory\n"
    "    QUIT <CRLF>                   : Disconnection\n"
    "    DELE <SP> <pathname> <CRLF>   : Delete file on the server\n"
    "    PWD  <CRLF>                   : Print working directory\n"
    "    PASV <CRLF>                   : Enable \"passive\" mode for data "
    "transfer\n"
    "    PORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data "
    "transfer\n"
    "    HELP [<SP> <string>] <CRLF>   : List available commands\n"
    "    NOOP <CRLF>                   : Do nothing\n"
    "    RETR <SP> <pathname> <CRLF>   : Download file from server to client\n"
    "    STOR <SP> <pathname> <CRLF>   : Upload file from client to server\n"
    "    LIST [<SP> <pathname>] <CRLF> : List files in the current working "
    "directory\r\n";

static const char *const help_arr[] = {
    "214 USER <SP> <username> <CRLF>   : Specify user for authentication\r\n",
    "214 PASS <SP> <password> <CRLF>   : Specify password for "
    "authentication\r\n",
    "214 CWD  <SP> <pathname> <CRLF>   : Change working directory\r\n",
    "214 CDUP <CRLF>                   : Change working directory to parent "
    "directory\r\n",
    "214 QUIT <CRLF>                   : Disconnection\r\n",
    "214 DELE <SP> <pathname> <CRLF>   : Delete file on the server\r\n",
    "214 PWD  <CRLF>                   : Print working directory\r\n",
    "214 PASV <CRLF>                   : Enable \"passive\" mode for data "
    "transfer\r\n",
    "214 PORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data "
    "transfer\r\n",
    "214 HELP [<SP> <string>] <CRLF>   : List available commands\r\n",
    "214 NOOP <CRLF>                   : Do nothing\r\n",
    "214 RETR <SP> <pathname> <CRLF>   : Download file from server to "
    "client\r\n",
    "214 STOR <SP> <pathname> <CRLF>   : Upload file from client to "
    "server\r\n",
    "214 LIST [<SP> <pathname>] <CRLF> : List files in the current working "
    "directory\r\n",
    NULL
};

void noop(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 0)
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
    else dprintf(ftp->tmp->socket, "200 Command okay.\r\n");
}

void quit(ftp_infos_t *ftp)
{
    ftp->tmp->quit = 1;
    dprintf(ftp->tmp->socket, "221 Goodbye.\r\n");
}

void help(ftp_infos_t *ftp)
{
    char *token = strtok(NULL, " \r\n");
    if (token != NULL) dprintf(1, " %s", token);
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    }
    if (token == NULL) {
        dprintf(ftp->tmp->socket, help_buf);
        return;
    }
    size_t len = strlen(token);
    for (int i = 0; help_arr[i] != NULL; i++) {
        if (strncmp(token, (help_arr[i] + 4), len) == 0) {
            dprintf(ftp->tmp->socket, help_arr[i]);
            return;
        }
    }
}

void pwd(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    }
    if (ftp->tmp->path != NULL)
        dprintf(ftp->tmp->socket, "257 %s\r\n", ftp->tmp->path);
    else
        dprintf(ftp->tmp->socket, "421 Service not available.\r\n");
}

void dele(ftp_infos_t *ftp)
{
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    }
    char *token = strtok(NULL, "\r\n");
    char tmp[PATH_MAX];
    getcwd(tmp, sizeof(tmp));
    chdir(ftp->tmp->path);
    printf("%s %s\n", token, ftp->buffer);
    if (token != NULL) dprintf(1, " %s", token);
    else {
        dprintf(ftp->tmp->socket, "501 Syntax error\r\n"), chdir(tmp);
        return;
    }
    int ret = remove(token);
    if (ret == 0)
        dprintf(ftp->tmp->socket, "250 \"%s\" successfully deleted\r\n",
            token);
    else dprintf(ftp->tmp->socket, "550 No such file\r\n");
    chdir(tmp);
}