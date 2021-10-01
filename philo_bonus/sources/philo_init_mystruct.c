/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_mystruct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 12:14:27 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 14:02:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static void	init_queue(t_philosophers *mystruct)
{
	int	i;

	philo_try_malloc((void **)&mystruct->semQueueNames,
		mystruct->phNum * sizeof(*mystruct->semQueueNames));
	philo_try_malloc((void **)&mystruct->semQueue,
		mystruct->phNum * sizeof(*mystruct->semQueue));
	philo_try_malloc((void **)&mystruct->semDoneEating,
		mystruct->phNum * sizeof(*mystruct->semDoneEating));
	philo_try_malloc((void **)&mystruct->semDoneEatingNames,
		mystruct->phNum * sizeof(*mystruct->semDoneEatingNames));
	i = -1;
	while (++i < mystruct->phNum)
	{
		mystruct->semQueueNames[i] = ft_strjoin_free(
				ft_strdup(SEM_QUEUE), ft_itoa(i + 1));
		mystruct->semQueue[i] = philo_sem_init(mystruct->semQueueNames[i], 1);
		mystruct->semDoneEatingNames[i] = ft_strjoin_free(
				ft_strdup(SEM_DONE_EATING), ft_itoa(i + 1));
		mystruct->semDoneEating[i] = philo_sem_init(
				mystruct->semDoneEatingNames[i], 1);
	}
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
	mystruct->phNum = philo_atoi(argv[1]);
	mystruct->forks = philo_sem_init(SEM_FORKS, mystruct->phNum);
	mystruct->semFinish = philo_sem_init(SEM_FINISH, 0);
	init_queue(mystruct);
	mystruct->semPrint = philo_sem_init(SEM_PRINT, 1);
	mystruct->semFinishedEating = philo_sem_init(SEM_FINISHED_EATING, 0);
	mystruct->time_to_die = philo_atoi(argv[2]);
	mystruct->time_to_eat = philo_atoi(argv[3]);
	mystruct->time_to_sleep = philo_atoi(argv[4]);
	mystruct->nOfMeals = CANT_STOP_EATING;
	if (argc == 6)
		mystruct->nOfMeals = philo_atoi(argv[5]);
	philo_try_malloc((void **)&mystruct->process_ids, mystruct->phNum);
	philo_try_calloc((void **)&mystruct->ph_arr, mystruct->phNum,
		sizeof(*mystruct->ph_arr));
	return (0);
}

static void	free_queue(t_philosophers *mystruct)
{
	int	i;

	sem_close(mystruct->semFinish);
	sem_close(mystruct->semFinishedEating);
	sem_close(mystruct->semPrint);
	i = -1;
	while (++i < mystruct->phNum)
	{
		sem_close(mystruct->semQueue[i]);
		sem_unlink(mystruct->semQueueNames[i]);
		sem_close(mystruct->semDoneEating[i]);
		sem_unlink(mystruct->semDoneEatingNames[i]);
		philo_my_free((void **)&mystruct->semDoneEatingNames[i]);
		philo_my_free((void **)&mystruct->semQueueNames[i]);
	}
	philo_my_free((void **)&mystruct->semQueue);
	philo_my_free((void **)&mystruct->semQueueNames);
	philo_my_free((void **)&mystruct->semDoneEating);
	philo_my_free((void **)&mystruct->semDoneEatingNames);
}

/*
** Frees all memory associated with 'mystruct'.
** Returns 1.
*/
int	philo_destroy_mystruct(t_philosophers *mystruct)
{
	philo_my_free((void **)&mystruct->process_ids);
	philo_my_free((void **)&mystruct->ph_arr);
	if (mystruct->allFinishedEating == true)
	{
		sem_post(mystruct->semFinish);
		pthread_join(mystruct->endCondThread, NULL);
	}
	free_queue(mystruct);
	sem_close(mystruct->forks);
	sem_unlink(SEM_FINISH);
	sem_unlink(SEM_FINISHED_EATING);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FORKS);
	return (1);
}
