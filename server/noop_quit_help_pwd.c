/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** noop
*/

#include "ftp.h"

static const char help_buf[] =
    "214\tUSER <SP> <username> <CRLF>   : Specify user for authentication\n"
    "\tPASS <SP> <password> <CRLF>   : Specify password for authentication\n"
    "\tCWD  <SP> <pathname> <CRLF>   : Change working directory\n"
    "\tCDUP <CRLF>                   : Change working directory to parent "
    "directory\n"
    "\tQUIT <CRLF>                   : Disconnection\n"
    "\tDELE <SP> <pathname> <CRLF>   : Delete file on the server\n"
    "\tPWD  <CRLF>                   : Print working directory\n"
    "\tPASV <CRLF>                   : Enable \"passive\" mode for data "
    "transfer\n"
    "\tPORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data "
    "transfer\n"
    "\tHELP [<SP> <string>] <CRLF>   : List available commands\n"
    "\tNOOP <CRLF>                   : Do nothing\n"
    "\tRETR <SP> <pathname> <CRLF>   : Download file from server to client\n"
    "\tSTOR <SP> <pathname> <CRLF>   : Upload file from client to server\n"
    "\tLIST [<SP> <pathname>] <CRLF> : List files in the current working "
    "directory\r\n";

static const char *const help_arr[] = {
    "214\tUSER <SP> <username> <CRLF>   : Specify user for authentication\r\n",
    "214\tPASS <SP> <password> <CRLF>   : Specify password for "
    "authentication\r\n",
    "214\tCWD  <SP> <pathname> <CRLF>   : Change working directory\r\n",
    "214\tCDUP <CRLF>                   : Change working directory to parent "
    "directory\r\n",
    "214\tQUIT <CRLF>                   : Disconnection\r\n",
    "214\tDELE <SP> <pathname> <CRLF>   : Delete file on the server\r\n",
    "214\tPWD  <CRLF>                   : Print working directory\r\n",
    "214\tPASV <CRLF>                   : Enable \"passive\" mode for data "
    "transfer\r\n",
    "214\tPORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data "
    "transfer\r\n",
    "214\tHELP [<SP> <string>] <CRLF>   : List available commands\r\n",
    "214\tNOOP <CRLF>                   : Do nothing\r\n",
    "214\tRETR <SP> <pathname> <CRLF>   : Download file from server to "
    "client\r\n",
    "214\tSTOR <SP> <pathname> <CRLF>   : Upload file from client to "
    "server\r\n",
    "214\tLIST [<SP> <pathname>] <CRLF> : List files in the current working "
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
        dprintf(ftp->tmp->socket, "257 \"%s\"\r\n", ftp->tmp->path);
    else
        dprintf(ftp->tmp->socket, "421 Service not available\r\n");
}