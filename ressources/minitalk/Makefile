NAME_SERVER := server
NAME_CLIENT := client
SRCDIR := src/
OBJDIR := obj/
SERVER_SRCS	:= $(addprefix $(SRCDIR), server.c server_utils.c)
CLIENT_SRCS	:= $(addprefix $(SRCDIR), client.c client_utils.c)
SERVER_OBJS := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SERVER_SRCS))
CLIENT_OBJS := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(CLIENT_SRCS))

CC := cc
CFLAGS := -Wall -Werror -Wextra -g -I inc

##################################################

all : $(NAME_SERVER) $(NAME_CLIENT)

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_SERVER) : $(SERVER_OBJS)
	make -C src/libft
	$(CC) $(SERVER_OBJS) $(CFLAGS) -o $(NAME_SERVER) src/libft/libft.a

server_san : $(SERVER_OBJS)
	make -C src/libft
	$(CC) $(SERVER_OBJS) $(CFLAGS) -fsanitize=address -o $(NAME_SERVER)_san src/libft/libft.a

$(NAME_CLIENT) : $(CLIENT_OBJS)
	make -C src/libft
	$(CC) $(CLIENT_OBJS) $(CFLAGS) -o $(NAME_CLIENT) src/libft/libft.a

clean:
	make -C src/libft clean
	rm -rf $(OBJDIR)

fclean : clean
	make -C src/libft fclean
	rm -f $(NAME_SERVER) $(NAME_CLIENT) $(NAME_SERVER)_san

re:	fclean all

.PHONY:	all clean fclean re
