CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
LFLAGS = -pthread
# CFLAGS += -g -fsanitize=thread
# LFLAGS += -fsanitize=thread
SDIR = sources
ODIR = objects
HDIR = headers
NAME = philo
SRC = philo_main.c philo_init_mystruct.c philo_utils.c philo_threads.c \
philo_philo_routine.c philo_nodbin_functions.c philo_queue_functions.c \
philo_libft_utils.c philo_reaper_routine.c philo_reaper_utils.c \
philo_philo_utils.c philo_malloc_utils.c philo_print_functions.c \
philo_time_utils.c

$(NAME): $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file))
	$(CC) $(LFLAGS) -o $@ $(foreach file,$(SRC:.c=.o),$(ODIR)/$(file))
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