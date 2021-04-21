##
## EPITECH PROJECT, 2021
## B-PSU-402-NCE-4-1-strace-marton.szuts
## File description:
## Makefile
##

NAME_SERVER	=	myftp

NAME_CLIENT	=	myclient

SRCS_SERVER	=	server/main.c \
				server/client_list.c \
				server/ftp_init.c \
				server/core.c \
				server/signal_handlers.c \
				server/commands.c \
				server/noop_user.c \
				server/sha256.c \
				server/sha256_to_string.c \
				server/quit.c \

SRCS_CLIENT =	client/main.c \

OBJS_SERVER	=	$(SRCS_SERVER:.c=.o)

OBJS_CLIENT	=	$(SRCS_CLIENT:.c=.o)

CFLAGS		=	-Wall -Wextra -g3 -O0

all:	$(NAME_SERVER)

server: $(NAME_SERVER)

client: $(NAME_CLIENT)

$(NAME_SERVER):	$(OBJS_SERVER) $(SRCS_SERVER)
		gcc -o $(NAME_SERVER) $(OBJS_SERVER)

$(NAME_CLIENT):	$(OBJS_CLIENT) $(SRCS_CLIENT)
		gcc -o $(NAME_CLIENT) $(OBJS_CLIENT)

clean:
		rm -rf $(OBJS_SERVER)
		rm -rf $(OBJS_CLIENT)

fclean:
		rm -rf $(OBJS_SERVER)
		rm -rf $(OBJS_CLIENT)
		rm -rf $(NAME_SERVER)
		rm -rf $(NAME_CLIENT)

re: fclean all