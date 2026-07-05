#==============================================================================#
#                                 Files and Paths                              #
#==============================================================================#

SRCS = $(addprefix $(SRCS_PATH)/, config/config_parser.cpp config/Config.cpp config/Location.cpp config/Server.cpp \
config/parse_utils.cpp config/parse_config_info.cpp core/main.cpp core/debug.cpp \
http/Request.cpp http/ResponseBuilder.cpp cgi/CgiHandler.cpp cgi/cgi_utils.cpp server/request_handler.cpp)

OBJS = $(patsubst $(SRCS_PATH)/%.cpp, $(BUILD_PATH)/%.o, $(SRCS))

NAME = webserv

BUILD_PATH = .build
SRCS_PATH = ./srcs

#==============================================================================#
#                                   Alias                                      #
#==============================================================================#

CXX = c++

RM = rm -rf
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98
SILENT_MAKE = make -s extra

#==============================================================================#
#                                    Rules                                     #
#==============================================================================#

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GRN)[ webserv successfully compiled]$(D)"

$(BUILD_PATH)/%.o: $(SRCS_PATH)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

clean: 
	@$(RM) $(BUILD_PATH)
	@echo "$(BCYA)[clean] Objects removed$(D)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(BCYA)[fclean] Archive removed$(D)"

re: fclean all

again: clean all

.PHONY: all clean fclean re again

#==============================================================================#
#                                  UTILS                                       #
#==============================================================================#

# Colors
#
# Run the following command to get list of available colors
# bash -c 'for c in {0..255}; do tput setaf $c; tput setaf $c | cat -v; echo =$c; done'
#
B  		= $(shell tput bold)
BLA		= $(shell tput setaf 0)
RED		= $(shell tput setaf 1)
GRN		= $(shell tput setaf 2)
YEL		= $(shell tput setaf 3)
BLU		= $(shell tput setaf 4)
MAG		= $(shell tput setaf 5)
CYA		= $(shell tput setaf 6)
WHI		= $(shell tput setaf 7)
GRE		= $(shell tput setaf 8)
BRED 	= $(shell tput setaf 9)
BGRN	= $(shell tput setaf 10)
BYEL	= $(shell tput setaf 11)
BBLU	= $(shell tput setaf 12)
BMAG	= $(shell tput setaf 13)
BCYA	= $(shell tput setaf 14)
BWHI	= $(shell tput setaf 15)
D 		= $(shell tput sgr0)
BEL 	= $(shell tput bel)
CLR 	= $(shell tput el 1)