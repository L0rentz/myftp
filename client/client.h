/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** client
*/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NONE 0
#define STOR 1
#define RETR 2

typedef struct s_data_socket {
    int port;
    int socket;
    int master_socket;
    struct sockaddr_in addr;
    int addr_len;
} data_socket_t;

typedef struct s_client_infos {
    int port;
    int ret;
    int socket;
    int val_read;
    int sending;
    int fd_send;
    int state;
    struct pollfd fds;
    struct sockaddr_in serv_addr;
    char buffer[1025];
    data_socket_t data_socket;
} client_infos_t;

int parse_buffer(client_infos_t *client);
int port_open(client_infos_t *client, int *check);
void stor_send(client_infos_t *client);
void data_transfer_selector(client_infos_t *client, int timeout);
int server_read_write(client_infos_t *client, int fd_read,
    int fd_write, int timeout);
int stor_check(client_infos_t *client);
void pasv_connect(client_infos_t *client);

#endif /* !CLIENT_H_ */