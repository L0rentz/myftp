/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** user_list
*/

#include "client_list.h"

void push_front_client(client_control_t *client_list, int socket)
{
    client_list_t *new_node = malloc(sizeof(client_list_t));
    new_node->socket = socket;
    new_node->data_socket = 0;
    new_node->master_socket = 0;
    new_node->next = NULL;
    new_node->path = NULL;
    new_node->quit = 0;
    new_node->exist = 0;
    new_node->transfer = 0;
    new_node->mode = NONE;
    memset(new_node->pass, '\0', 65);
    memset(new_node->username, '\0', 65);
    new_node->logged = 0;
    new_node->user_cmd = 0;
    if (client_list->head != NULL) new_node->next = client_list->head;
    client_list->head = new_node;
    client_list->count++;
}

void pop_disconnected_clients(client_control_t *client_list)
{
    client_list_t *tmp = client_list->head;
    while (tmp != NULL && tmp->socket == 0) {
        tmp = tmp->next;
        if (client_list->head->path != NULL) free(client_list->head->path);
        free(client_list->head);
        client_list->head = tmp;
        client_list->count--;
    }
    client_list_t *to_delete;
    for (; tmp != NULL && tmp->next != NULL; tmp = tmp->next) {
        if (tmp->next->socket == 0) {
            to_delete = tmp->next;
            tmp->next = to_delete->next;
            if (to_delete->path != NULL) free(to_delete->path);
            free(to_delete);
            client_list->count--;
        }
    }
}

client_control_t *clean_client_control(client_control_t *client_list)
{
    if (client_list == NULL) return (NULL);
    client_list_t *tmp = client_list->head;
    client_list_t *to_delete = NULL;
    while (tmp != NULL && tmp->next != NULL) {
        to_delete = tmp;
        tmp = tmp->next;
        if (to_delete->path != NULL) free(to_delete->path);
        free(to_delete);
        client_list->count--;
    }
    if (tmp != NULL) {
        if (tmp->path) free(tmp->path);
        free(tmp);
        client_list->count--;
        client_list->head = NULL;
    }
    free(client_list);
    return (NULL);
}

void print_clients_sockets(client_list_t **head)
{
    for (client_list_t *tmp = *head; tmp != NULL; tmp = tmp->next) {
        if (tmp->next == NULL) printf("%d\n", tmp->socket);
        else printf("%d, ", tmp->socket);
    }
}

client_control_t *init_client_control(void)
{
    client_control_t *client_list = malloc(sizeof(client_control_t));
    client_list->head = NULL;
    client_list->count = 0;
    return (client_list);
}