/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_mystruct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 12:14:27 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:35:49 by edavid           ###   ########.fr       */
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

	if (philo_try_malloc((void **)&mystruct->forks, mystruct->phNum
			* sizeof(*mystruct->forks)))
		return (1);
	i = -1;
	while (++i < mystruct->phNum)
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
	int					i;
	t_philosopher_info	*cur_philo;

	i = -1;
	while (++i < mystruct->phNum)
	{
		cur_philo = &mystruct->ph_arr[i];
		cur_philo->phNum = i + 1;
		cur_philo->time_to_die = mystruct->time_to_die * 1000;
		cur_philo->time_to_eat = mystruct->time_to_eat * 1000;
		cur_philo->time_to_sleep = mystruct->time_to_sleep * 1000;
		cur_philo->nOfMeals = mystruct->nOfMeals;
		if (i == mystruct->phNum - 1)
			cur_philo->refLeftFork = &mystruct->forks[0];
		else
			cur_philo->refLeftFork = &mystruct->forks[i + 1];
		cur_philo->refRightFork = &mystruct->forks[i];
		cur_philo->refStartTime = &mystruct->start_time;
		cur_philo->meal_queue = &mystruct->meal_queue;
		cur_philo->ref_queue = &mystruct->time_left_lst_mutex;
		cur_philo->refStartMutex = &mystruct->start_mutexes[i];
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
	if (philo_try_malloc((void **)&mystruct->start_mutexes,
			mystruct->phNum * sizeof(*mystruct->start_mutexes)))
		return (1);
	i = -1;
	while (++i < mystruct->phNum)
		if (pthread_mutex_init(&mystruct->start_mutexes[i], NULL))
			return (1);
	return (0);
}

/*
** Initializes 'mystruct' from the program arguments.
** Returns 0 on success, 1 on failure.
** By convention nOfMeals is set to -1 if the
** corresponding argument is not provided.
** All the values are initialized to 0 if not set explicitly.
*/
int	philo_init_mystruct(t_philosophers *mystruct, int argc, char **argv)
{
	memset(mystruct, 0, sizeof(*mystruct));
	philo_get_mystruct(mystruct);
	mystruct->game_over = false;
	mystruct->phNum = philo_atoi(argv[1]);
	mystruct->time_to_die = philo_atoi(argv[2]);
	mystruct->time_to_eat = philo_atoi(argv[3]);
	mystruct->time_to_sleep = philo_atoi(argv[4]);
	mystruct->nOfMeals = CANT_STOP_EATING;
	if (argc == 6)
		mystruct->nOfMeals = philo_atoi(argv[5]);
	if (philo_try_calloc((void **)&mystruct->ph_arr,
			mystruct->phNum, sizeof(*mystruct->ph_arr)))
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

	philo_my_free((void **)&mystruct->ph_arr);
	i = -1;
	while (++i < mystruct->phNum)
	{
		pthread_mutex_destroy(&mystruct->forks[i].fork);
		pthread_mutex_destroy(&mystruct->forks[i].is_available_mutex);
		pthread_mutex_destroy(&mystruct->start_mutexes[i]);
	}
	philo_my_free((void **)&mystruct->forks);
	ft_nodbinclear(&mystruct->meal_queue, ft_nodbindel, -1);
	pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
	pthread_mutex_destroy(&mystruct->time_left_lst_mutex);
	pthread_mutex_unlock(&mystruct->print_mutex);
	pthread_mutex_destroy(&mystruct->print_mutex);
	philo_my_free((void **)&mystruct->start_mutexes);
	memset(mystruct, 0, sizeof(*mystruct));
	return (1);
}
