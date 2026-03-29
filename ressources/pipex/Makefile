# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 18:40:53 by pswirgie          #+#    #+#              #
#    Updated: 2026/03/27 15:26:27 by pswirgie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ================= VARIABLES ================= #

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g 
MAKEFLAGS	+= --no-print-directory
DATE		:= $(shell date +"%y_%m_%d_%H-%M-%S")
BUILD_DIR	:= .pipex

# Includes
INCLUDES	:=							\
				-Iincludes				\
				-Ilib/libft/			\

NAME		:= pipex

# Colors
GREEN		:='\033[0;32m'
NC			:='\033[0m'

# Sources
SRCS		:= 											\
			src/get_next_line/end_gnl.c					\
			src/get_next_line/get_next_line.c			\
			src/cmd_options.c							\
			src/error_free.c							\
			src/exec_cmds.c								\
			src/fill_tab.c								\
			src/init_args.c								\
			src/init_files.c							\
			src/init.c									\
			src/main.c									\
			src/path_explicit.c							\
			src/utils.c									\

OBJS		:= $(SRCS:src/%.c=$(BUILD_DIR)/%.o)


# ==================  LIBS  ================== #

# GNL
DIR_GNL	:= lib/get_next_line
GNL		:= lib/get_next_line/get_next_line.a

# Libft
DIR_LIB	:= lib/libft
LIBFT	:= lib/libft/libft.a



# ================= COMMANDS ================= #

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(DIR_LIB)

$(NAME): $(BUILD_DIR) $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) -o $(NAME)
	@echo $(GREEN)"\n✨ Pipex build created. ✨\n"$(NC)
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
	@echo $(GREEN)"Pipex build is clean. 🧹"$(NC)

fclean: clean
	@$(MAKE) -C $(DIR_LIB) fclean
	@rm -f $(NAME)
	@echo $(GREEN)"Pipex library is clean. 🧹"$(NC)

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
