# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rpedrosa & mreinald                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/19 00:00:00 by rpedrosa          #+#    #+#              #
#    Updated: 2026/07/19 00:00:00 by rpedrosa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED		= \033[0;31m
GREEN	= \033[0;32m
BLUE	= \033[0;34m
D_BLUE	= \033[34m
WHITE	= \033[0;37m
YELLOW	= \033[0;33m
MAGENTA	= \033[0;35m
CYAN	= \033[3;36m
RESET	= \033[0m

# Program's name
NAME		= bin/webserv

# Directories
SRC_DIR		= ./srcs
OBJ_DIR		= objs
BIN_DIR		= bin
INC_DIR		= ./includes

# Compiler and Flags
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g
INCLUDES	= -I$(INC_DIR)

# Handle different OS configurations
# (no external libs here — this only picks the right leak checker)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LEAK_CMD	= valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
endif
ifeq ($(UNAME_S),Darwin)
	LEAK_CMD	= leaks --atExit --
endif

# Sources and Objects
SRCS		= $(addprefix $(SRC_DIR)/, \
				cgi/CgiHandler.cpp \
				cgi/cgi_utils.cpp \
				config/config_parser.cpp \
				config/Config.cpp \
				config/Location.cpp \
				config/parse_config_info.cpp \
				config/parse_utils.cpp \
				config/Server.cpp \
				core/debug.cpp \
				core/main.cpp \
				http/Request.cpp \
				http/ResponseBuilder.cpp \
				server/Client.cpp \
				server/request_handler.cpp \
				)
OBJS		= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRCS:.cpp=.o))

# Default config used by `make run` / `make leaks`
CONFIG		?= ./config/webserv.conf

# ASCII Art
define ART
$(MAGENTA)${D_BLUE}
	██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗
	██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║
	██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║
	██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝
	╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝
	 ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝
  $(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━[$(RESET)HTTP Server in C++98$(CYAN)]━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)
endef
export ART

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@clear
	@echo "$$ART"
	@echo "$(CYAN)webserv compiled successfully! $(RESET)-> $(GREEN)$(NAME)$(RESET)"
	@echo "$(YELLOW)Run: ./$(NAME) $(CONFIG)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@sleep 0.02
	@clear
	@echo "$(RED)Compiling webserv sources $(RESET)$<"

# Build then launch with a config file:  make run CONFIG=config/other.conf
run: $(NAME)
	@echo "$(YELLOW)Starting webserv with $(CONFIG)...$(RESET)"
	@./$(NAME) $(CONFIG)

# Leak check — valgrind on Linux, leaks on macOS
leaks: $(NAME)
	@echo "$(YELLOW)Leak check ($(UNAME_S))...$(RESET)"
	@$(LEAK_CMD) ./$(NAME) $(CONFIG)

clean:
	@clear
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Object files removed!$(RESET)"

fclean: clean
	@rm -rf $(BIN_DIR)
	@echo "$(GREEN)webserv removed!$(RESET)"

re: fclean all

again: clean all

.PHONY: all clean fclean re again run leaks
