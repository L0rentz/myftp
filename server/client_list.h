/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** client_list
*/

#ifndef CLIENT_LIST_H_
#define CLIENT_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct s_client_list {
    int socket;
    char pass[65];
    char username[65];
    int logged;
    int exist;
    int quit;
    struct s_client_list *next;
} client_list_t;

typedef struct s_client_control {
    int count;
    client_list_t *head;
} client_control_t;

void push_front_client(client_control_t *client_list, int socket);
void pop_disconnected_clients(client_control_t *client_list);
client_control_t *init_client_control();
void print_clients_sockets(client_list_t **head);
client_control_t *clean_client_control(client_control_t *client_list);

#endif /* !CLIENT_LIST_H_ */