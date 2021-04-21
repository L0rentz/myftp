/*
** EPITECH PROJECT, 2021
** B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
** File description:
** sha256_to_string
*/

#include <stdio.h>
#include "sha256.h"

char *sha256_to_string(char *to_found)
{
    char *sha256 = malloc(sizeof(char) * 65);
    BYTE text1[strlen(to_found)];
    snprintf((char *)text1, strlen(to_found) + 1, "%s", to_found);
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;;
    sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen((const char *)text1));
	sha256_final(&ctx, buf);
    char *tmp = sha256;
    for (int i = 0; i < 32; i++, tmp += 2)
        snprintf(tmp, 3, "%02x", buf[i]);
    sha256[64] = '\0';
    return (sha256);
}