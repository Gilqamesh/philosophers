/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:13:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 19:02:53 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Creates threads for each philosopher.
** Returns 0 on success, 1 on failure.
*/
int	philo_init_threads(t_philosophers *mystruct, t_threads *threads)
{
	int	i;
	int	ret;

	if (philo_try_malloc((void **)&threads->philosopher_threads,
		mystruct->n_of_philosophers * sizeof(*threads->philosopher_threads)))
		return (philo_destroy_mystruct(mystruct));
	i = -1;
	while (++i < mystruct->n_of_philosophers)
		pthread_mutex_lock(&mystruct->start_mutexes[i]);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		ret = pthread_create(&threads->philosopher_threads[i], NULL,
				&philo_routine, &mystruct->array_of_philosophers[i]);
		if (ret)
		{
			PRINT_HERE();
			return (philo_destroy_mystruct(mystruct));
		}
	}
	ret = pthread_create(&threads->reaper_thread, NULL,
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
int	philo_join_threads(t_philosophers *mystruct, t_threads *threads)
{
	int	ret;
	int	i;

	ret = pthread_join(threads->reaper_thread, NULL);
	printf("Reaper has finished\n");
	if (ret)
	{
		PRINT_HERE();
		return (ret);
	}
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		ret = pthread_join(threads->philosopher_threads[i], NULL);
		printf("Philosopher number (%d) has finished\n", i + 1);
		if (ret)
		{
			PRINT_HERE();
			return (ret);
		}
	}
	return (0);
}
