# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g -fsanitize=address # -g for debugging, remove for release
# If you are using the readline library from a non-standard location, you might need:
# CFLAGS += -I/path/to/readline/include
# LDFLAGS += -L/path/to/readline/lib
LDFLAGS = -lreadline # Common linker flag for readline

# Project Name
NAME = minishell

# Directories
INCLUDE_DIR = ./include/
SRC_DIR = ./src/
SHELL_DIR = $(SRC_DIR)shell/
TOKEN_DIR = $(SRC_DIR)tokenization/
EXP_DIR = $(SRC_DIR)expansion/
ENV_DIR = $(SRC_DIR)env_handling/
HEREDOC_DIR = $(SRC_DIR)heredoc/
SIGNAL_DIR = $(SRC_DIR)signal_handling/
UTIL_DIR = $(SRC_DIR)utils/
EXUCUTION = $(SRC_DIR)exucution/
PARSER = $(SRC_DIR)parser/
# Source Files
# Note: Order doesn't strictly matter for SRCS, but it's good for organization.
SRCS = \
	$(SHELL_DIR)main.c \
	$(SHELL_DIR)shell_1.c \
	$(SHELL_DIR)shell_2.c \
	$(SHELL_DIR)check.c \
	$(SHELL_DIR)cleanup.c \
	$(TOKEN_DIR)tokenizer_main.c \
	$(TOKEN_DIR)tokenizer_core.c \
	$(TOKEN_DIR)tokenizer_special.c \
	$(TOKEN_DIR)handling_words.c \
	$(TOKEN_DIR)handling_words_utils.c \
	$(TOKEN_DIR)is_x.c \
	$(EXP_DIR)parsing_main.c \
	$(EXP_DIR)expanding_main.c \
	$(EXP_DIR)expand_token_value.c \
	$(EXP_DIR)expansion_process.c \
	$(EXP_DIR)expansion_context.c \
	$(EXP_DIR)expansion_splitting.c \
	$(EXP_DIR)distribute_split.c \
	$(EXP_DIR)expansion_error.c \
	$(EXP_DIR)process_each_token_utils.c \
	$(EXP_DIR)log_var_main.c \
	$(EXP_DIR)var_logging_utils.c \
	$(EXP_DIR)expansion_utils1.c \
	$(EXP_DIR)expansion_utils2.c \
	$(EXP_DIR)expanding_added.c \
	$(ENV_DIR)environement_main.c \
	$(ENV_DIR)environement_utils.c \
	$(ENV_DIR)environement_utils2.c \
	$(SIGNAL_DIR)handle_signals.c \
	$(UTIL_DIR)str_stuff1.c \
	$(UTIL_DIR)str_stuff2.c \
	$(UTIL_DIR)str_stuff3.c \
	$(UTIL_DIR)str_stuff4.c \
	$(UTIL_DIR)ft_split.c \
	$(HEREDOC_DIR)expand_heredoc_line.c \
	$(HEREDOC_DIR)generate_heredoc_filename.c \
	$(HEREDOC_DIR)handle_heredocs.c \
	$(HEREDOC_DIR)read_heredoc.c \
	$(EXUCUTION)builtin.c \
	$(EXUCUTION)builtin2.c \
	$(EXUCUTION)export.c \
	$(EXUCUTION)export2.c \
	$(EXUCUTION)export3.c \
	$(EXUCUTION)exucutable.c \
	$(EXUCUTION)execution_main.c \
	$(EXUCUTION)rediractions2.c \
	$(EXUCUTION)unset.c \
	$(EXUCUTION)ft_atoi.c \
	$(EXUCUTION)ft_lstadd_back.c \
	$(EXUCUTION)ft_lstsize.c \
	$(EXUCUTION)ft_putstr_fd.c \
	$(EXUCUTION)ft_strtrim.c \
	$(EXUCUTION)execution_helper.c \
	$(EXUCUTION)execution_helper2.c \
	$(PARSER)parser.c \
	$(PARSER)parse_single_command.c \
	$(PARSER)parser_utils.c 
# Object Files
OBJS = $(SRCS:.c=.o)

# Include header directory
CFLAGS += -I$(INCLUDE_DIR)

# Default rule
all: $(NAME)

# Rule to link object files into the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "Minishell compiled!"

# Rule to compile .c files into .o files
%.o: %.c $(INCLUDE_DIR)minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule: removes object files
clean:
	rm -f $(OBJS)
	@echo "Object files cleaned."

# Fclean rule: removes object files and the executable
fclean: clean
	rm -f $(NAME)
	@echo "All cleaned."

# Re rule: forces recompilation
re: fclean all

# Phony targets: targets that are not actual files
.PHONY: all clean fclean re
