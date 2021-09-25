/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:33:50 by edavid            #+#    #+#             */
/*   Updated: 2021/09/25 22:56:27 by edavid           ###   ########.fr       */
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

enum e_philo_status
{
	PHILO_THINKING,
	PHILO_EATING,
	PHILO_SLEEPING
};

typedef struct s_philo_fork
{
	pthread_mutex_t	fork;
	bool			is_available;
	pthread_mutex_t	is_available_mutex;
}	t_philo_fork;

// time_to_die, time_to_eat and time_to_sleep should be converted to microsecs
typedef struct s_philosopher_info
{
	int					philosopher_number;
	enum e_philo_status	current_status;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_meals_needed;
	t_philo_fork		*reference_to_set_of_forks;
	t_philo_fork		*reference_to_left_fork;
	t_philo_fork		*reference_to_right_fork;
	struct timeval		*reference_to_start_time;
	int					*reference_to_time_left_array_position;
	pthread_mutex_t		*reference_to_time_left_array_mutex;
	pthread_mutex_t		*reference_to_start_mutex;
}	t_philosopher_info;

// time_to_die, time_to_eat and time_to_sleep are in milliseconds
typedef struct s_philosophers
{
	int					n_of_philosophers;
	t_philosopher_info	*array_of_philosophers;
	pthread_t			*philosopher_threads;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					n_of_times_each_philosopher_must_eat;
	t_philo_fork		*forks;
	t_philo_fork		*pair_of_forks;
	struct timeval		start_time;
	pthread_mutex_t		*start_mutexes;
	int					*time_left_till_starvation_array;
	pthread_mutex_t		time_left_array_mutex;
	pthread_t			reaper_thread;
}	t_philosophers;

// Initializing functions

int		philo_init_mystruct(t_philosophers *mystruct, int argc, char **argv);
int		philo_destroy_mystruct(t_philosophers *mystruct);

// Thread functions

int		philo_init_threads(t_philosophers *mystruct);
int		philo_join_threads(t_philosophers *mystruct);

// Routines

void	*philo_routine(void *info);
void	*reaper_routine(void *mystructPtr);
void	*start_routine(void *mystructPtr);

// Utils functions

int		philo_my_free(void **pointer);
int		philo_atoi(char *str);
bool	philo_is_valid_positive_number(char *str);
int		philo_calc_ms_difference(struct timeval *current, struct timeval *previous);
void	*philo_calloc(int number_of_elements, int size_of_each_element);
int		philo_try_calloc(void **ret, int n_of_els, int size_of_el);
int		philo_try_malloc(void **ret, int size);

#endif
