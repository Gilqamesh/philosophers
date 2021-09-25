/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:13:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/25 22:56:38 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Creates threads for each philosopher.
** Returns 0 on success, 1 on failure.
*/
int	philo_init_threads(t_philosophers *mystruct)
{
	int	i;
	int	ret;

	i = -1;
	while (++i < mystruct->n_of_philosophers)
		pthread_mutex_lock(&mystruct->start_mutexes[i]);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		ret = pthread_create(&mystruct->philosopher_threads[i], NULL,
				&philo_routine, &mystruct->array_of_philosophers[i]);
		if (ret)
		{
			PRINT_HERE();
			return (philo_destroy_mystruct(mystruct));
		}
	}
	ret = pthread_create(&mystruct->reaper_thread, NULL,
		&reaper_routine, mystruct);
	if (ret)
	{
		PRINT_HERE();
		return (philo_destroy_mystruct(mystruct));
	}
	return (0);
}

/*
** Waits for the threads to finish execution.
** Returns 0 on success, 1 on failure.
*/
int	philo_join_threads(t_philosophers *mystruct)
{
	int	i;
	int	ret;

	ret = pthread_join(mystruct->reaper_thread, NULL);
	if (ret)
	{
		PRINT_HERE();
		return (philo_destroy_mystruct(mystruct));
	}
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		ret = pthread_detach(mystruct->philosopher_threads[i]);
		if (ret)
		{
			PRINT_HERE();
			return (philo_destroy_mystruct(mystruct));
		}
	}
	return (0);
}
