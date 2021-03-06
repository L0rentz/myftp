##
## EPITECH PROJECT, 2021
## B-NWP-400-NCE-4-1-myftp-lorentz-andrea.romeo
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
				server/user_pass.c \
				server/sha256.c \
				server/sha256_to_string.c \
				server/list.c \
				server/error.c \
				server/noop_quit_help_pwd_dele.c \
				server/pasv.c \
				server/cwd_cdup.c \
				server/port.c \
				server/retr.c \
				server/stor.c \

SRCS_CLIENT =	client/main.c \
				client/parse_buffer.c \
				client/port.c \
				client/stor.c \

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