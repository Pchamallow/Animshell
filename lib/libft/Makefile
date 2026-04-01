# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 18:40:53 by pswirgie          #+#    #+#              #
#    Updated: 2026/02/15 17:29:12 by pswirgie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
FLAGS	:= -Wall -Wextra -Werror -g
DATE	:= $(shell date +"%y_%m_%d_%H-%M-%S")
NAME	:= libft.a
FILES	:= \
	ft_atol.c \
	ft_bzero.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isalnum.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_memset.c \
	ft_memcpy.c \
	ft_strlen.c \
	ft_memmove.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_toupper.c \
	ft_tolower.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_memchr.c \
	ft_memcmp.c\
	ft_strnstr.c \
	ft_itoa.c \
	ft_strdup.c \
	ft_calloc.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c\
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstclear.c \
	ft_printchar.c\
	ft_printf_fd.c\
	ft_printpercent.c\
	ft_putpointer.c\
# 	ft_lstdelone.c \
# 	ft_lstiter.c \
# 	ft_lstmap.c \

OBJET	:= $(FILES:.c=.o)

%.o : %.c
	@$(CC) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME): $(OBJET)
	@$(AR) rcs $@ $(OBJET)
	@echo "Libft.a is created"

clean :
	@rm -rf $(OBJET)

fclean : clean
	@rm -rf $(NAME)

re : fclean
	$(MAKE) all

zip:
	zip DATE_HOUR.zip *
	mv DATE_HOUR.zip $(DATE).zip
	mv $(DATE).zip 00_ARCHIVES

unzip:
	unzip *.zip -d $(DATE)

setup :
	mkdir 00_ARCHIVES

.PHONY : all clean fclean re setup unzip zip
