/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:13:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:25:55 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Creates a thread for each philosopher.
** Then creates the thread for the 'reaper' process.
** Returns 0 on success, 1 on failure.
*/
int	philo_init_threads(t_philosophers *mystruct, t_threads *threads)
{
	int	i;

	if (philo_try_malloc((void **)&threads->phThreads,
			mystruct->phNum * sizeof(*threads->phThreads)))
		return (philo_destroy_mystruct(mystruct));
	i = -1;
	while (++i < mystruct->phNum)
		pthread_mutex_lock(&mystruct->start_mutexes[i]);
	i = -1;
	while (++i < mystruct->phNum)
		pthread_create(&threads->phThreads[i], NULL,
			&philo_routine, &mystruct->ph_arr[i]);
	pthread_create(&threads->reaper_thread, NULL, &reaper_routine, mystruct);
	return (0);
}

/*
** Waits for the threads to finish execution.
*/
int	philo_join_threads(t_philosophers *mystruct, t_threads *threads)
{
	int	i;

	i = -1;
	while (++i < mystruct->phNum)
		pthread_join(threads->phThreads[i], NULL);
	pthread_mutex_lock(&mystruct->game_over_mutex);
	mystruct->game_over = true;
	pthread_mutex_unlock(&mystruct->game_over_mutex);
	pthread_join(threads->reaper_thread, NULL);
	return (0);
}
