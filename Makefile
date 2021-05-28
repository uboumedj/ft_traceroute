NAME =		ft_traceroute
CC =		clang

CFLAGS =	-Wall -Wextra -Werror

SRCS =		src/main.c src/flags.c src/address.c src/socket.c src/signal.c \
			src/loop.c src/time.c src/display.c src/utils.c

INC =		ft_traceroute.h

OBJS =		$(SRCS:.c=.o)


.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(CFLAGS) -o $(NAME) $(SRCS) -lm
	@echo "\033[1;32mprogram...compiled\t✓\033[0m"

clean:
	@rm -rf $(OBJS)
	@echo "\033[1;34mft_traceroute\033[1;33m obj files removed\t\033[1;31m✓\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;34mft_traceroute\033[1;33m files deleted\t\t\033[1;31m✓\033[0m"

re: fclean all