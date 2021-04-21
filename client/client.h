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

typedef struct s_client_infos {
    int port;
    int ret;
    int socket;
    int val_read;
    struct pollfd fds;
    struct sockaddr_in serv_addr;
    char buffer[1025];
} client_infos_t;

#endif /* !CLIENT_H_ */