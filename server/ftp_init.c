/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** ftp_init
*/

#include "ftp.h"

ftp_infos_t *init_ftp_infos()
{
    ftp_infos_t *ftp = malloc(sizeof(ftp_infos_t));
    ftp->client_list = init_client_control();
    ftp->port = PORT;
    ftp->opt = TRUE;
    ftp->message = "220 Service ready for new user.\r\n";
    memset(&ftp->sa, '\0', sizeof(ftp->sa));
    sigemptyset(&ftp->sa.sa_mask);
    ftp->sa.sa_flags = 0;
    ftp->sa.sa_handler = sigint_handler;
    return (ftp);
}

static void bind_listen(ftp_infos_t *ftp)
{
    ftp->address.sin_family = AF_INET;
    ftp->address.sin_addr.s_addr = INADDR_ANY;
    ftp->address.sin_port = htons(ftp->port);
    if (bind(ftp->master_socket, (struct sockaddr *)&ftp->address,
    sizeof(ftp->address)) < 0) {
        perror("bind failed");
        longjmp(s_jumpBuffer, 84);
    }
    printf("Listener on port %d\n", ftp->port);
    if (listen(ftp->master_socket, 3) < 0) {
        perror("listen");
        longjmp(s_jumpBuffer, 84);
    }
}

void init_ftp_server(ftp_infos_t *ftp)
{
    signal(SIGPIPE, SIG_IGN);
    if ((ftp->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        longjmp(s_jumpBuffer, 84);
    }
    if (setsockopt(ftp->master_socket, SOL_SOCKET, SO_REUSEADDR,
    (char *)&ftp->opt, sizeof(ftp->opt)) < 0) {
        perror("setsockopt");
        longjmp(s_jumpBuffer, 84);
    }
    bind_listen(ftp);
    ftp->addr_len = sizeof(ftp->address);
    puts("Waiting for connections...");
}