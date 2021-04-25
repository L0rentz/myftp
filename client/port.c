/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** port
*/

#include "client.h"

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
    token = strtok(NULL, "\r\n");
    if (token == NULL) return (-1);
    strcat(port2, token);
    token = strtok(NULL, "\r\n");
    if (token != NULL) return (-1);
    *port = atoi(port1) * 256 + atoi(port2);
    return (0);
}

static int port_bind_listen_accept(client_infos_t *client, int *check)
{
    if (bind(client->data_socket.master_socket, (struct sockaddr *)
    &client->data_socket.addr, sizeof(client->data_socket.addr)) < 0) {
        perror("Bind failed");
        return (-1);
    } if (listen(client->data_socket.master_socket, 1) < 0) {
        perror("listen failed");
        return (-1);
    } client->data_socket.addr_len = sizeof(client->data_socket.addr);
    dprintf(client->socket, client->buffer);
    *check = 1;
    if ((client->data_socket.socket = accept(client->data_socket.master_socket,
    (struct sockaddr *)&client->data_socket.addr,
    (socklen_t *)&client->data_socket.addr_len)) < 0) {
        perror("accept");
        return (-1);
    } return (0);
}

static int port_init(client_infos_t *client, int *check)
{
    int enable = 1;
    setsockopt(client->data_socket.master_socket, SOL_SOCKET, SO_REUSEADDR,
        &enable, sizeof(int));
    char buf[100] = {0};
    int port = 0;
    char *token = strtok(NULL, ",\r\n");
    if (token == NULL) {
        printf("Bad argument.\n");
        return (-1);
    } if (port_get_ip(token, &port, buf) == -1) {
        printf("Bad argument.\n");
        return (-1);
    } if (inet_pton(AF_INET, buf, &client->data_socket.addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        return (-1);
    }
    client->data_socket.addr.sin_family = AF_INET;
    client->data_socket.addr.sin_addr.s_addr = INADDR_ANY;
    client->data_socket.addr.sin_port = htons(port);
    return (port_bind_listen_accept(client, check));
}

int port_open(client_infos_t *client, int *check)
{
    char *tmp = strdup(client->buffer);
    char *token = strtok(tmp, " \r\n");
    if (token != NULL && strlen(token) != 4) {
        if (tmp != NULL) free(tmp);
        return (0);
    } if (client->data_socket.socket != 0) close(client->data_socket.socket);
    if (client->data_socket.master_socket != 0)
        close(client->data_socket.master_socket);
    if ((client->data_socket.master_socket =
    socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        if (tmp != NULL) free(tmp);
        perror("socket failed");
        return (-1);
    }
    int ret = port_init(client, check);
    if (tmp != NULL) free(tmp);
    return (ret);
}