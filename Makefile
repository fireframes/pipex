NAME = pipex

CFLAGS = -Wextra -Wall -Werror

HEADERS = -Iinclude

LIBFT = ./lib/libft.a
LIBFTPRINTF = ./lib/libftprintf.a
LIBS = $(LIBFT) $(LIBFTPRINTF)

SRCS_DIR= ./src/
SRCS = $(SRCS_DIR)pipex.c \
		$(SRCS_DIR)get_cmdargs.c \
		$(SRCS_DIR)pipex_utils.c

OBJS = ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	@echo "Compiling $< to $@..."
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(LIBFT) $(LIBFTPRINTF) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "Linking object files to create $(NAME)..."
	@echo "\n'$(NAME)' binary successfully created."

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJS)

fclean: clean
	@echo "\nPerforming full clean..."
	rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
