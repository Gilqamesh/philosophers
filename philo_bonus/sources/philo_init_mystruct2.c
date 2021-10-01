/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_mystruct2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 12:05:35 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 14:00:16 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static void	init_helper(t_philosophers *mystruct, t_philosopher_info *cur_philo,
int i)
{
	cur_philo->nOfMeals = mystruct->nOfMeals;
	cur_philo->startTime = mystruct->startTime;
	cur_philo->ateTimestamp = mystruct->startTime;
	cur_philo->semQueue = mystruct->semQueue[i];
	cur_philo->semQueueName = mystruct->semQueueNames[i];
	cur_philo->semDoneEating = mystruct->semDoneEating[i];
	cur_philo->semDoneEatingName = mystruct->semDoneEatingNames[i];
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
		if (mystruct->phNum % 2 == 0)
			cur_philo->time_to_think = 0;
		else
		{
			if (cur_philo->time_to_eat - cur_philo->time_to_sleep + 1000 < 0)
				cur_philo->time_to_think = 0;
			else
				cur_philo->time_to_think
					= cur_philo->time_to_eat - cur_philo->time_to_sleep + 1000;
		}
		init_helper(mystruct, cur_philo, i);
	}
}

int	philo_init_processes(t_philosophers *mystruct)
{
	int	i;

	mystruct->startTime = philo_get_current_timestamp()
		+ INIT_TIME_IN_MICROSECONDS;
	init_philosophers_array(mystruct);
	i = -1;
	while (++i < mystruct->phNum)
	{
		mystruct->process_ids[i] = fork();
		if (mystruct->process_ids[i] == 0)
			philo_process(mystruct, &mystruct->ph_arr[i]);
	}
	return (0);
}

int	philo_kill_processes(t_philosophers *mystruct)
{
	int	i;

	mystruct->allFinishedEating = true;
	i = -1;
	while (++i < mystruct->phNum)
		sem_wait(mystruct->semFinishedEating);
	if (mystruct->allFinishedEating == true)
		sem_post(mystruct->semFinish);
	pthread_join(mystruct->endCondThread, NULL);
	i = -1;
	while (++i < mystruct->phNum)
	{
		if (mystruct->allFinishedEating == true)
		{
			sem_post(mystruct->semQueue[i]);
			waitpid(mystruct->process_ids[i], NULL, -1);
		}
		else
			kill(mystruct->process_ids[i], SIGTERM);
	}
	return (0);
}
