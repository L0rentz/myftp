/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** parse_buffer
*/

#include "client.h"

static void pasv_get_ip_port(client_infos_t *client, char *buf)
{
    char *token = strtok(client->buffer, "(");
    token = strtok(NULL, ",");
    strcat(buf, token), strcat(buf, ".");
    token = strtok(NULL, ",");
    strcat(buf, token), strcat(buf, ".");
    token = strtok(NULL, ",");
    strcat(buf, token), strcat(buf, ".");
    token = strtok(NULL, ",");
    strcat(buf, token);
    char port1[10] = {0}, port2[10] = {0};
    token = strtok(NULL, ",");
    strcat(port1, token);
    token = strtok(NULL, ")");
    strcat(port2, token);
    client->data_socket.port = atoi(port1) * 256 + atoi(port2);
}

static void pasv_connect(client_infos_t *client)
{
    if ((client->data_socket.socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return;
    }
    char buf[100] = {0};
    pasv_get_ip_port(client, buf);
    client->data_socket.addr.sin_family = AF_INET;
    client->data_socket.addr.sin_port = htons(client->data_socket.port);
    if (inet_pton(AF_INET, buf, &client->data_socket.addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        return;
    } if (connect(client->data_socket.socket, (struct sockaddr *)
    &client->data_socket.addr, sizeof(client->data_socket.addr)) < 0) {
        printf("Connection failed\n");
        return;
    }
}

void parse_buffer(client_infos_t *client)
{
    if (strncmp(client->buffer, "227", 3) == 0)
        pasv_connect(client);
    if (strncmp(client->buffer, "221", 3) == 0) {
        free(client);
        exit(0);
    }
}