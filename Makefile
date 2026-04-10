# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 18:40:53 by pswirgie          #+#    #+#              #
#    Updated: 2026/04/08 22:11:48 by stkloutz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ================= VARIABLES ================= #

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g
LDFLAGS		:= -lreadline
MAKEFLAGS	+= --no-print-directory
DATE		:= $(shell date +"%y_%m_%d_%H-%M-%S")
BUILD_DIR	:= .minishell

# Includes
INCLUDES	:=							\
				-Iincludes				\
				-Ilib/libft/			\

NAME		:= minishell

# Colors
GREEN		:='\033[0;32m'
NC			:='\033[0m'

# Sources
SRCS		:= 											\
			src/execute/is_cmd/is_cmd.c					\
			src/execute/is_cmd/is_path.c				\
			src/execute/utils.c							\
			src/execute/error_free.c					\
			src/execute/execute.c						\
			src/execute/launch_display.c				\
			src/execute/term.c							\
			src/parsing/ft_token_list.c					\
			src/parsing/handle_token_types.c			\
			src/parsing/separate_into_tokens.c			\
			src/parsing/parsing_utils.c					\
			src/parsing/parsing.c						\
			src/execute/build_in/echo.c					\
			src/main.c									\


OBJS		:= $(SRCS:src/%.c=$(BUILD_DIR)/%.o)


# ==================  LIBS  ================== #

# Libft
DIR_LIB	:= lib/libft
LIBFT	:= lib/libft/libft.a



# ================= COMMANDS ================= #

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(DIR_LIB)

$(NAME): $(BUILD_DIR) $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) $(LDFLAGS) -o $(NAME)
	@echo $(GREEN)"\n✨ Animshell created. ✨\n"$(NC)
	@clear
	@echo $(GREEN)"💫 All compiled 💫\n"$(NC)

$(BUILD_DIR):
	@mkdir -p $@

# Compilation .c -> .o
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) -C $(DIR_LIB) clean
	@rm -rf $(BUILD_DIR)
	@echo $(GREEN)"Animshell build is clean. 🧹"$(NC)

fclean: clean
	@$(MAKE) -C $(DIR_LIB) fclean
	@rm -f $(NAME)
	@echo $(GREEN)"Animshell library is clean. 🧹"$(NC)

re: fclean 
	$(MAKE) all



# ================= Archives ================= #

zip:
	@zip DATE_HOUR.zip *
	@mv DATE_HOUR.zip ${DATE}.zip
	@mv ${DATE}.zip 00_ARCHIVES
	@echo "${GREEN}All is zipped.${NC}"

unzip:
	@unzip *.zip -d ${DATE}
	@echo "${GREEN}All is unzipp.${NC}"

setup :
	@mkdir 00_ARCHIVES
	@echo "${GREEN}Setup is good !${NC}"

.PHONY: all clean fclean re setup unzip zip
