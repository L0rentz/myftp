/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** stor
*/

#include "client.h"

int stor_check(client_infos_t *client)
{
    if (client->sending == 0) {
        char *token = strtok(client->buffer, " \r\n");
        token = strtok(NULL, " \r\n");
        if (token == NULL) {
            printf("No filepath.\n");
            client->state = NONE;
            return (-1);
        }
        client->fd_send = open(token, O_RDONLY);
        if (client->fd_send < 0) {
            printf("File does not exist.\n");
            client->state = NONE;
            return (-1);
        } client->sending = 1;
    } return (0);
}

void stor_send(client_infos_t *client)
{
    if (client->state != STOR || client->sending == 0) return;
    int valread = 0;
    char buf[1025] = {0};
    while ((valread = read(client->fd_send, buf, 1024)) > 0) {
        buf[valread] = '\0';
        write(client->data_socket.socket, buf, valread);
    }
    close(client->fd_send);
    client->sending = 0;
    client->state = NONE;
}