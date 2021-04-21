/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** main
*/

#include "ftp.h"

jmp_buf s_jumpBuffer;

int main(int ac, char **av)
{
    (void)ac, (void)av;
    int val = 0;
    ftp_infos_t *ftp = init_ftp_infos();
    if (sigaction(SIGINT, &ftp->sa, NULL) == -1) {
        perror("sigaction");
        return (84);
    }
    if ((val = setjmp(s_jumpBuffer))) {
        clean_client_control(ftp->client_list);
        free(ftp);
        return (val);
    }
    init_ftp_server(ftp);
    server_loop(ftp);
    return (0);
}