/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** core
*/

#include "ftp.h"

void create_fd_set(ftp_infos_t *ftp)
{
    FD_ZERO(&ftp->readfds);
    FD_SET(ftp->master_socket, &ftp->readfds);
    ftp->max_sd = ftp->master_socket;
    for (client_list_t *tmp = ftp->client_list->head;
    tmp != NULL; tmp = tmp->next) {
        FD_SET(tmp->socket, &ftp->readfds);
        if (tmp->socket > ftp->max_sd) ftp->max_sd = tmp->socket;
    }
}

void incoming_connection(ftp_infos_t *ftp)
{
    if (FD_ISSET(ftp->master_socket, &ftp->readfds)) {
        if ((ftp->new_socket = accept(ftp->master_socket,
        (struct sockaddr *)&ftp->address, (socklen_t *)&ftp->addr_len)) < 0) {
            perror("accept");
            longjmp(s_jumpBuffer, 84);
        }
        printf("New connection, socket fd is %d, ip is : %s, port : %d\n",
            ftp->new_socket, inet_ntoa(ftp->address.sin_addr),
            ntohs(ftp->address.sin_port));
        if (write(ftp->new_socket, ftp->message, strlen(ftp->message))
        != (ssize_t)strlen(ftp->message))
            perror("write");
        puts("Connection message sent successfully");
        push_front_client(ftp->client_list, ftp->new_socket);
        printf("Adding to list of sockets as %d\n", ftp->client_list->count);
    }
}

void closing_responding(ftp_infos_t *ftp)
{
    if ((ftp->val_read = read(ftp->tmp->socket, ftp->buffer, 1024)) == 0) {
        getpeername(ftp->tmp->socket, (struct sockaddr *)&ftp->address,
            (socklen_t *)&ftp->addr_len);
        printf("Host disconnected, ip %s, port %d\n",
            inet_ntoa(ftp->address.sin_addr), ntohs(ftp->address.sin_port));
        close(ftp->tmp->socket);
        ftp->tmp->socket = 0;
    }
    else {
        ftp->buffer[ftp->val_read] = '\0';
        is_command(ftp);
    }
}

void sockets_operations(ftp_infos_t *ftp)
{
    for (client_list_t *tmp = ftp->client_list->head;
    tmp != NULL; tmp = tmp->next) {
        if (FD_ISSET(tmp->socket, &ftp->readfds)) {
            ftp->tmp = tmp;
            closing_responding(ftp);
        }
    }
}

void server_loop(ftp_infos_t *ftp)
{
    while (1) {
        create_fd_set(ftp);
        ftp->activity =
            select(ftp->max_sd + 1, &ftp->readfds, NULL, NULL, NULL);
        if ((ftp->activity < 0) && (errno != EINTR)) perror("select");
        incoming_connection(ftp);
        sockets_operations(ftp);
        pop_disconnected_clients(ftp->client_list);
    }
}