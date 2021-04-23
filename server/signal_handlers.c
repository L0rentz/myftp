/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** signal_handlers
*/

#include "ftp.h"

void sigint_handler(int sig)
{
    (void)sig;
    dprintf(1, "\nClosing gracefully...\n");
    longjmp(s_jumpBuffer, 84);
}