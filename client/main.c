/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** main
*/

#include "client.h"

void init_client_socket(client_infos_t *client, char **av)
{
    if ((client->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        free(client);
        exit(84);
    }
    client->serv_addr.sin_family = AF_INET;
    client->serv_addr.sin_port = htons(client->port);
    if (inet_pton(AF_INET, av[1], &client->serv_addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        free(client);
        exit(84);
    }
}

client_infos_t *init_client_infos(char **av)
{
    client_infos_t *client = malloc(sizeof(client_infos_t));
    client->port = atoi(av[2]);
    if (client->port <= 0 || client->port > 65535) {
        free(client);
        exit(84);
    }
    client->fds.fd = 1;
	client->fds.events = POLLIN;
    client->ret = 0;
    client->socket = 0;
    client->val_read = 0;
    return (client);
}

void connect_to_server(client_infos_t *client, char **av)
{
    printf("Trying %s...\n", av[1]);
    if (connect(client->socket, (struct sockaddr *)&client->serv_addr,
    sizeof(client->serv_addr)) < 0) {
        printf("Connection failed\n");
        free(client);
        exit(84);
    }
    printf("Connected to %s.\n", av[1]);
    client->val_read = read(client->socket, client->buffer, 1024);
    if (client->val_read != 0)
        write(1, client->buffer, strlen(client->buffer));
}

int server_read_write(client_infos_t *client, int fd_read,
    int fd_write, int timeout)
{
    client->fds.fd = fd_read;
    client->ret = poll(&client->fds, 1, timeout);
	if (client->ret == -1) {
	    perror("poll");
	    free(client);
        exit(84);
	}
    memset(client->buffer, '\0', 1024);
    if (client->ret > 0)
        client->val_read = read(fd_read, client->buffer, 1024);
    client->buffer[client->val_read] = '\0';
    write(fd_write, client->buffer, strlen(client->buffer));
    if (client->ret > 0)
        return (-1);
    return (0);
}

int main(int ac, char **av)
{
    if (ac != 3) return (84);
    client_infos_t *client = init_client_infos(av);
    init_client_socket(client, av);
    connect_to_server(client, av);
    int timeout = 0;
    while (1) {
        server_read_write(client, 1, client->socket, timeout);
        server_read_write(client, client->socket, 1, timeout);
    }
    return (0);
}