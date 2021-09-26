/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_mystruct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 12:14:27 by edavid            #+#    #+#             */
/*   Updated: 2021/09/26 19:51:15 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Initializes the forks in 'mystruct' as mutexes.
** Returns 0 on success, 1 on failure.
** Deallocates 'mystruct' in case of failure before returning.
*/
static int	init_forks(t_philosophers *mystruct)
{
	int	i;

	if (philo_try_malloc((void **)&mystruct->forks, mystruct->n_of_philosophers
		* sizeof(*mystruct->forks)))
		return (1);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		if (pthread_mutex_init(&mystruct->forks[i].fork, NULL)
			|| pthread_mutex_init(&mystruct->forks[i].is_available_mutex, NULL))
			return (1);
		mystruct->forks[i].is_available = true;
	}
	return (0);
}

/*
** Initializes the arra of philosophers in 'mystruct'.
** All the values are set to default.
*/
static void	init_philosophers_array(t_philosophers *mystruct)
{
	int	i;

	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		mystruct->array_of_philosophers[i].philosopher_number = i + 1;
		mystruct->array_of_philosophers[i].time_to_die
			= mystruct->time_to_die * 1000;
		mystruct->array_of_philosophers[i].time_to_eat
			= mystruct->time_to_eat * 1000;
		mystruct->array_of_philosophers[i].time_to_sleep
			= mystruct->time_to_sleep * 1000;
		mystruct->array_of_philosophers[i].number_of_meals_needed
				= mystruct->n_of_times_each_philosopher_must_eat;
		mystruct->array_of_philosophers[i].set_of_forks.is_available = true;
		mystruct->array_of_philosophers[i].set_of_forks.reference_to_right_fork = &mystruct->forks[i];
		if (i == mystruct->n_of_philosophers - 1)
		{
			mystruct->array_of_philosophers[i].set_of_forks.reference_to_left_fork
				= &mystruct->forks[0];
			mystruct->array_of_philosophers[i].reference_to_left_fork
				= &mystruct->forks[0];
		}
		else
		{
			mystruct->array_of_philosophers[i].set_of_forks.reference_to_left_fork
				= &mystruct->forks[i + 1];
			mystruct->array_of_philosophers[i].reference_to_left_fork
				= &mystruct->forks[i + 1];
		}
		pthread_mutex_init(&mystruct->array_of_philosophers[i]
				.set_of_forks.is_available_mutex, NULL);
		mystruct->array_of_philosophers[i].reference_to_right_fork
			= &mystruct->forks[i];
		mystruct->array_of_philosophers[i].reference_to_start_time
				= &mystruct->start_time;
		mystruct->array_of_philosophers[i].time_left_till_starvation_lst
				= &mystruct->time_left_till_starvation_lst;
		mystruct->array_of_philosophers[i].reference_to_time_left_lst_mutex
				= &mystruct->time_left_lst_mutex;
		mystruct->array_of_philosophers[i].reference_to_start_mutex
				= &mystruct->start_mutexes[i];
	}
}

/*
** Allocates and initializes the mutexes in 'mystruct'.
** Returns 0 on success, 1 on failure.
*/
static int	philo_init_mutexes(t_philosophers *mystruct)
{
	int	i;

	if (pthread_mutex_init(&mystruct->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&mystruct->time_left_lst_mutex, NULL))
		return (1);
	if (philo_try_malloc((void **)&mystruct->start_mutexes, mystruct->n_of_philosophers
		* sizeof(*mystruct->start_mutexes)))
		return (1);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
		if (pthread_mutex_init(&mystruct->start_mutexes[i], NULL))
			return (1);
	return (0);
}

/*
** Initializes 'mystruct' from the program arguments.
** Returns 0 on success, 1 on failure.
** By convention n_of_times_each_philosopher_must_eat is set to -1 if the
** corresponding argument is not provided.
** All the values are initialized to 0 if not set explicitly.
*/
int	philo_init_mystruct(t_philosophers *mystruct, int argc, char **argv)
{
	memset(mystruct, 0, sizeof(*mystruct));
	mystruct->n_of_philosophers = philo_atoi(argv[1]);
	mystruct->time_to_die = philo_atoi(argv[2]);
	mystruct->time_to_eat = philo_atoi(argv[3]);
	mystruct->time_to_sleep = philo_atoi(argv[4]);
	mystruct->n_of_times_each_philosopher_must_eat = CANT_STOP_EATING;
	if (argc == 6)
		mystruct->n_of_times_each_philosopher_must_eat = philo_atoi(argv[5]);
	if (philo_try_calloc((void **)&mystruct->array_of_philosophers,
		mystruct->n_of_philosophers, sizeof(*mystruct->array_of_philosophers)))
		return (philo_destroy_mystruct(mystruct));
	if (philo_try_malloc((void **)&mystruct->philosopher_threads,
		mystruct->n_of_philosophers * sizeof(*mystruct->philosopher_threads)))
		return (philo_destroy_mystruct(mystruct));
	if (init_forks(mystruct))
		return (philo_destroy_mystruct(mystruct));
	if (philo_init_mutexes(mystruct))
		return (philo_destroy_mystruct(mystruct));
	init_philosophers_array(mystruct);
	return (0);
}

/*
** Frees all memory associated with 'mystruct'.
** Returns 1.
*/
int	philo_destroy_mystruct(t_philosophers *mystruct)
{
	int	i;

	i = -1;
	while (++i < mystruct->n_of_philosophers)
		pthread_mutex_destroy(&mystruct->array_of_philosophers[i]
			.set_of_forks.is_available_mutex);
	philo_my_free((void **)&mystruct->array_of_philosophers);
	philo_my_free((void **)&mystruct->philosopher_threads);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		pthread_mutex_destroy(&mystruct->forks[i].fork);
		pthread_mutex_destroy(&mystruct->forks[i].is_available_mutex);
	}
	philo_my_free((void **)&mystruct->forks);
	// ft_nodbinprint(mystruct->time_left_till_starvation_lst);
	ft_nodbinclear(&mystruct->time_left_till_starvation_lst, ft_nodbindel, -1);
	pthread_mutex_unlock(&mystruct->print_mutex);
	pthread_mutex_destroy(&mystruct->print_mutex);
	pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
	pthread_mutex_destroy(&mystruct->time_left_lst_mutex);
	memset(mystruct, 0, sizeof(*mystruct));
	return (1);
}
