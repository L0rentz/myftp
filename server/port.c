/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** port
*/

#include "ftp.h"

static int port_get_ip(char *token, int *port, char *buf)
{
    char port1[10] = {0}, port2[10] = {0};
    strcat(buf, token), strcat(buf, ".");
    for (int i = 0; i < 3; i++) {
        token = strtok(NULL, ",");
        if (token == NULL) return (-1);
        strcat(buf, token);
        if (i != 2) strcat(buf, ".");
    }
    token = strtok(NULL, ",");
    if (token == NULL) return (-1);
    strcat(port1, token);
    token = strtok(NULL, ")");
    if (token == NULL) return (-1);
    strcat(port2, token);
    token = strtok(NULL, "\r\n");
    if (token != NULL) return (-1);
    *port = atoi(port1) * 256 + atoi(port2);
    return (0);
}

static int port_connect(ftp_infos_t *ftp)
{
    int port = 0;
    char buf[100] = {0};
    char *token = strtok(NULL, ",\r\n");
    if (token == NULL) return (-1);
    if (port_get_ip(token, &port, buf) == -1) {
        return (-1);
    }
    printf(" %s %d\n", buf, port);
    ftp->tmp->address.sin_family = AF_INET;
    ftp->tmp->address.sin_port = htons(port);
    if (inet_pton(AF_INET, buf, &ftp->tmp->address.sin_addr) <= 0) return (-1);
    dprintf(ftp->tmp->socket, "200 Command okay.\r\n");
    if (connect(ftp->tmp->data_socket, (struct sockaddr *)
    &ftp->tmp->address, sizeof(ftp->tmp->address)) < 0) return (-1);
    return (0);
}

void port(ftp_infos_t *ftp)
{
    if (ftp->tmp->data_socket != 0) close(ftp->tmp->data_socket);
    if (ftp->tmp->master_socket != 0) close(ftp->tmp->master_socket);
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    }
    if ((ftp->tmp->data_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\nsocket failed");
        dprintf(ftp->tmp->socket, "421 Can't open data connection.\r\n");
        return;
    } if (port_connect(ftp) == -1) {
        dprintf(ftp->tmp->socket, "421 Can't open data connection.\r\n");
        return;
    } ftp->tmp->mode = ACTIVE;
}