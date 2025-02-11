# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/12 19:29:26 by mreinald          #+#    #+#              #
#    Updated: 2025/01/12 19:34:06 by mreinald         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Colors
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
MAGENTA	= \033[0;35m
CYAN	= \033[3;36m
RESET	= \033[0m

#Program's name
NAME	= bin/pipex

# Commands/flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I ./include -I ./lib/Libft/include

# Directories
SRC_DIR		= srcs
OBJ_DIR		= objs
BIN_DIR		= bin
LIBFT_DIR	= ./lib/Libft/

# Find Sources code
SRCS	= $(wildcard $(SRC_DIR)/*/*.c)
OBJS	= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRCS:.c=.o))

# Libft
LIBFT		= $(LIBFT_DIR)/libft.a
LIBFT_FLAGS	= -L./$(LIBFT_DIR) -lft

# ASCII Art
define ART
$(MAGENTA)
██████╗ ██╗██████╗ ███████╗██╗  ██╗
██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝
██████╔╝██║██████╔╝█████╗   ╚███╔╝ 
██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗ 
██║     ██║██║     ███████╗██╔╝ ██╗
╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝
  $(CYAN)━━━━━━━━━━━━[$(RESET)Made by $(BLUE)@tmatheusdiniz$(CYAN)]━━━━━━━━━━━━$(RESET)
endef
export ART

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) $(LIBFT_FLAGS) -o $(NAME)
	@clear
	@echo "$$ART"
	@echo "$(CYAN)pipex compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@sleep 0.02
	@clear
	@echo "$(RED)Compiling pipex sources $<$(RESET)"

$(LIBFT):
	@make --silent -C $(LIBFT_DIR)

clean:
	@clear
	@make --silent -C $(LIBFT_DIR) clean
	@rm -rf $(BIN_DIR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@clear
	@make --silent -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
