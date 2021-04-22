/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** ftp
*/

#ifndef FTP_H_
#define FTP_H_

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <setjmp.h>
#include <dirent.h>

#include "client_list.h"
#include "sha256.h"
#include "error.h"

#define TRUE 1
#define FALSE 0

extern jmp_buf s_jumpBuffer;

typedef struct s_ftp_infos {
    int opt;
    int master_socket;
    int addr_len;
    int new_socket;
    int activity;
    int val_read;
    int max_sd;
    int port;
    char *message;
    char *arg_path;
    char buffer[1025];
    fd_set readfds;
    struct sockaddr_in address;
    client_control_t *client_list;
    client_list_t *tmp;
    struct sigaction sa;
} ftp_infos_t;

ftp_infos_t *init_ftp_infos(int port, char path[]);
void init_ftp_server(ftp_infos_t *ftp);
int jump_handling(ftp_infos_t *ftp);
void server_loop(ftp_infos_t *ftp);
void sigint_handler(int sig);
void is_command(ftp_infos_t *ftp);
int check_args(int ac, char **av);

void noop(ftp_infos_t *ftp);
void user(ftp_infos_t *ftp);
void pass(ftp_infos_t *ftp);
void quit(ftp_infos_t *ftp);
void list(ftp_infos_t *ftp);

#endif /* !FTP_H_ */