/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** pasv
*/

#include "ftp.h"

void make_pasv_str(ftp_infos_t *ftp, char *buf, int port)
{
    char *ip = inet_ntoa(ftp->address.sin_addr);
    uint16_t port1 = (port - port % 256) / 256;
    uint16_t port2 = port % 256;
    sprintf(buf, "227 Entering Passive Mode (%s,%u,%u).\r\n",
        ip, port1, port2);
    for (int i = 0; buf[i] != '\0'; i++)
        if (buf[i] == '.' && buf[i + 1] != '\r')
            buf[i] = ',';
}

int pasv_accept(ftp_infos_t *ftp, int port)
{
    ftp->tmp->addr_len = sizeof(ftp->tmp->address);
    char buf[100] = {0};
    make_pasv_str(ftp, buf, port);
    dprintf(ftp->tmp->socket, buf);
    if ((ftp->tmp->data_socket = accept(ftp->tmp->master_socket,
    (struct sockaddr *)&ftp->tmp->address,
    (socklen_t *)&ftp->tmp->addr_len)) < 0) {
        perror("\naccept");
        dprintf(ftp->tmp->socket, "425 Can't open data connection.\r\n");
        return (1);
    }
    return (0);
}

int pasv_bind_port_listen(ftp_infos_t *ftp)
{
    int port = 1;
    for (; port < 65536; port += 1) {
        ftp->tmp->address.sin_port = htons(port);
        if (bind(ftp->tmp->master_socket, (struct sockaddr *)&ftp->tmp->address,
        sizeof(ftp->tmp->address)) == 0)
            break;
    }
    if (port > 65535) {
        perror("\nbind failed");
        dprintf(ftp->tmp->data_socket, "425 Can't open data connection.\r\n");
        return (1);
    } if (listen(ftp->tmp->master_socket, 1) < 0) {
        perror("\nlisten");
        dprintf(ftp->tmp->socket, "425 Can't open data connection.\r\n");
        return (1);
    }
    return (pasv_accept(ftp, port));
}

void pasv(ftp_infos_t *ftp)
{
    if (ftp->tmp->data_socket != 0) close(ftp->tmp->data_socket);
    if (ftp->tmp->master_socket != 0) close(ftp->tmp->master_socket);
    if (ftp->tmp->logged == 0) {
        dprintf(ftp->tmp->socket, "530 Please login with USER and PASS.\r\n");
        return;
    } if ((ftp->tmp->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("\nsocket failed");
        dprintf(ftp->tmp->socket, "425 Can't open data connection.\r\n");
        return;
    }
    int enable = 1;
    setsockopt(ftp->tmp->master_socket, SOL_SOCKET, SO_REUSEADDR,
        &enable, sizeof(int));
    ftp->tmp->address.sin_family = AF_INET;
    ftp->tmp->address.sin_addr.s_addr = INADDR_ANY;
    if (pasv_bind_port_listen(ftp) == 1) return;
    ftp->tmp->mode = PASSIVE;
}