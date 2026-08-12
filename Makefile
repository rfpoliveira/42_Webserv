# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/19 00:00:00 by rpedrosa          #+#    #+#              #
#    Updated: 2026/08/10 15:24:00 by rpedrosa         ###   ########.fr        #
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

# Auto-collect every subdirectory under includes/ as an -I path
INC_DIRS	= $(shell find $(INC_DIR) -type d)
INCLUDES	= $(addprefix -I,$(INC_DIRS))

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
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
#SRCS		= $(addprefix $(SRC_DIR)/, \
				cgi/CgiHandler.cpp \
				cgi/cgi_utils.cpp \
				config/config_parser.cpp \
				config/Config.cpp \
				config/Location.cpp \
				config/parseConfigInfo.cpp \
				config/parse_utils.cpp \
				config/ServerBlock.cpp \
				core/debug.cpp \
				core/main.cpp \
				exceptions/ConfigException.cpp \
				exceptions/HttpException.cpp \
				http/Request.cpp \
				http/ResponseBuilder.cpp \
				http/MimeTypes.cpp \
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
  $(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━[$(RESET)HTTP ServerBlock in C++98$(CYAN)]━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)
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
