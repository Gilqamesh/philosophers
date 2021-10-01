/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 18:00:47 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 13:55:38 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static void	exit_philo_process(t_philosophers *mystruct,
t_philosopher_info *pinfo)
{
	pthread_join(pinfo->reaper, NULL);
	sem_close(mystruct->forks);
	sem_close(mystruct->semFinish);
	sem_close(mystruct->semPrint);
	sem_close(mystruct->semFinishedEating);
	sem_close(pinfo->semQueue);
	sem_close(pinfo->semDoneEating);
	ft_nodbinclear(&pinfo->meal_queue, ft_nodbindel, -1);
	exit(EXIT_SUCCESS);
}

static void	init_philo_process(t_philosophers *mystruct,
t_philosopher_info *pinfo)
{
	pinfo->stopDeath = false;
	sem_open(SEM_FORKS, O_CREAT, 0);
	sem_open(SEM_FINISH, O_CREAT, 0);
	sem_open(SEM_FINISHED_EATING, O_CREAT, 0);
	sem_open(pinfo->semQueueName, O_CREAT, 0);
	sem_open(SEM_PRINT, O_CREAT, 0);
	sem_open(pinfo->semDoneEatingName, O_CREAT, 0);
	philo_enqueue(pinfo, pinfo->startTime);
	if (pthread_create(&pinfo->reaper, NULL, &reaper_routine, pinfo))
		exit_philo_process(mystruct, pinfo);
	philo_sleep_until_timestamp(pinfo->startTime);
	pinfo->lastThinkTimeStamp = pinfo->startTime;
	if (pinfo->phNum % 2 == 0)
	{
		pinfo->lastThinkTimeStamp += pinfo->time_to_eat;
		philo_sleep(pinfo, pinfo->startTime);
		philo_sleep_until_timestamp(pinfo->time_to_eat);
	}
	else
		philo_print_status(pinfo->phNum, PHTH, pinfo->startTime);
}

void	philo_process(t_philosophers *mystruct, t_philosopher_info *pinfo)
{
	init_philo_process(mystruct, pinfo);
	while (1)
	{
		philo_fork(mystruct, pinfo);
		philo_eat(mystruct, pinfo, pinfo->ateTimestamp);
		philo_sleep(pinfo, pinfo->ateTimestamp + pinfo->time_to_eat);
		philo_think(pinfo, pinfo->ateTimestamp + pinfo->time_to_eat
			+ pinfo->time_to_sleep);
		if (pinfo->nOfMeals != CANT_STOP_EATING
			&& pinfo->nOfMeals == 0)
		{
			sem_post(mystruct->semFinishedEating);
			sem_wait(pinfo->semQueue);
			exit_philo_process(mystruct, pinfo);
		}
	}
	exit_philo_process(mystruct, pinfo);
}
