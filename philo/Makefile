CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
LFLAGS = -pthread
SDIR = sources
ODIR = objects
HDIR = headers
NAME = philo
SRC = philo_main.c philo_init_mystruct.c philo_utils.c philo_threads.c \
philo_routine.c

$(NAME): $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file))
	$(CC) -o $@ $(LFLAGS) $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file))
$(ODIR)/%.o: $(SDIR)/%.c $(HDIR)/ft_philosophers.h
	cd $(ODIR) && $(CC) $(CFLAGS) -c ../$<

.PHONY: all clean fclean re bonus
all:
	make $(NAME)
clean:
	rm -f $(ODIR)/*.o
fclean:
	make clean
	rm -f $(NAME)
re:
	make clean
	make all
bonus:
	make $(NAME)