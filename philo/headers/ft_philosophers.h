/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:33:50 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 17:15:55 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <stdbool.h>
# define CANT_STOP_EATING -1
// debugging
# define PRINT_HERE() (printf("Line: %d, file: %s\n", __LINE__, __FILE__))
# define PRINT_PHILO() (printf("Line: %d, file: %s, philo number: (%d)\n", __LINE__, __FILE__, pinfo->philosopher_number))

enum e_philosopher_status
{
	PHILO_IS_EATING,
	PHILO_IS_SLEEPING,
	PHILO_IS_THINKING,
	PHILO_FORK,
	PHILO_FORK2,
	PHILO_DIED
};

typedef struct s_node_binary
{
	void					*content;
	struct s_node_binary	*next;
	struct s_node_binary	*prev;
}	t_node_binary;

// 'last_meal_timestamp' is in milliseconds
typedef struct s_philo_eat_info
{
	int			philosopher_index;
	long int	last_meal_timestamp;
}	t_philo_eat_info;

typedef struct s_philo_fork
{
	pthread_mutex_t	fork;
	bool			is_available;
	pthread_mutex_t	is_available_mutex;
}	t_philo_fork;

typedef struct s_philo_set_of_forks
{
	t_philo_fork	*reference_to_left_fork;
	t_philo_fork	*reference_to_right_fork;
	bool			is_available;
	pthread_mutex_t	is_available_mutex;
}	t_philo_set_of_forks;

// time_to_die, time_to_eat and time_to_sleep should be converted to microsecs
typedef struct s_philosopher_info
{
	int						philosopher_number;
	long int				time_to_die;
	long int				time_to_eat;
	long int				time_to_sleep;
	int						number_of_meals_needed;
	t_philo_fork			*reference_to_left_fork;
	t_philo_fork			*reference_to_right_fork;
	struct timeval			*reference_to_start_time;
	long int				last_meal_timestamp;
	pthread_mutex_t			*reference_to_meal_timestamps_mutex;
	t_node_binary			**meal_timestamps;
	pthread_mutex_t			*reference_to_start_mutex;
}	t_philosopher_info;

// time_to_die, time_to_eat and time_to_sleep are in milliseconds
typedef struct s_philosophers
{
	int					n_of_philosophers;
	t_philosopher_info	*array_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					n_of_times_each_philosopher_must_eat;
	t_philo_fork		*forks;
	struct timeval		start_time;
	pthread_mutex_t		*start_mutexes;
	t_node_binary		*meal_timestamps;
	t_node_binary		*first_in_queue;
	pthread_mutex_t		time_left_lst_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		game_over_mutex;
	bool				game_over;
}	t_philosophers;

typedef struct s_threads
{
	pthread_t		reaper_thread;
	pthread_t		*philosopher_threads;
	int				n_of_philosophers;
	pthread_mutex_t	detach_mutex;
}	t_threads;

// Initializing functions

int					philo_init_mystruct(t_philosophers *mystruct, int argc, char **argv);
int					philo_destroy_mystruct(t_philosophers *mystruct);

// Thread functions

int					philo_init_threads(t_philosophers *mystruct,
						t_threads *threads);
int					philo_join_threads(t_philosophers *mystruct,
						t_threads *threads);
int					philo_detach_threads(t_philosophers *mystruct,
						t_threads *threads);

// Routines

void				*philo_routine(void *info);
void				*reaper_routine(void *mystructPtr);
void				*start_routine(void *mystructPtr);

// Utils functions

int					philo_my_free(void **pointer);
int					philo_atoi(char *str);
bool				philo_is_valid_positive_number(char *str);
long int			philo_calc_microseconds_difference(struct timeval *current,
						struct timeval *previous);
long int			philo_add_timeval(struct timeval *t1, struct timeval *t2);
long int			philo_timval_to_microseconds(struct timeval *t);
void				*philo_calloc(int number_of_elements, int size_of_each_element);
int					philo_try_calloc(void **ret, int n_of_els, int size_of_el);
int					philo_try_malloc(void **ret, int size);
t_philo_eat_info	*philo_new_philo_info(int philo_index, long int timestamp);
t_philosophers		*philo_get_mystruct(t_philosophers *mystruct);
void				philo_enqueue(t_philosopher_info *pinfo,
						struct timeval *cur_time);
bool				philo_is_game_over(t_philosophers *mystruct);
void				philo_unlock_all_forks(t_philosophers *mystruct);
long int			long_int_max_of(long int a, long int b);

// t_node_binary functions

t_node_binary		*ft_nodbinnew(void *content);
void				ft_nodbinadd_front(t_node_binary **lst, t_node_binary *new);
void				ft_nodbinclear(t_node_binary **lst, void (*del)(void *), int n);
void				ft_nodbindelone(t_node_binary *lst, void (*del)(void *));
void				ft_nodbindel(void *node);
void				ft_nodbinprint(t_node_binary *lst);

// queue ADT functions

void				ft_fifonodbinenqueue(t_node_binary **lst, t_node_binary *new);
t_node_binary		*ft_fifonodbindequeue(t_node_binary **lst,
						t_node_binary *nullable_last_node);
bool				ft_fifolst_is_empty(t_node_binary *lst);

// print functions

int					philo_print_status(int philosopher_number,
						enum e_philosopher_status, struct timeval *cur_time);

#endif
