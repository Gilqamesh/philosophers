/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 18:00:47 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 21:08:03 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static int	exit_philo_process(t_philosophers *mystruct,
t_philosopher_info *pinfo, int ret)
{
	if (ret != 1)
		pthread_join(pinfo->reaper, NULL);
	sem_close(mystruct->semFinish);
	sem_close(mystruct->semFinishedEating);
	sem_close(mystruct->semPrint);
	sem_close(mystruct->semQueue);
	sem_close(mystruct->forks);
	return (ret);
}

int	philo_process(t_philosophers *mystruct, t_philosopher_info *pinfo)
{
	if (pthread_create(&pinfo->reaper, NULL, &reaper_routine, pinfo))
		return (exit_philo_process(mystruct, pinfo, 1));
	printf("Pid: %d\n", getpid());
	printf("Hello from child number %d\n", pinfo->phNum);
	usleep(100);
	printf("Hello from child number %d\n", pinfo->phNum);
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
	while (1)
	{
		philo_fork(mystruct, pinfo);
		philo_eat(pinfo, pinfo->ateTimestamp);
		if (pinfo->nOfMeals == 0)
		{
			sem_post(mystruct->semFinishedEating);
			return (exit_philo_process(mystruct, pinfo, 0));
		}
		philo_sleep(pinfo, pinfo->ateTimestamp + pinfo->time_to_eat);
		philo_think(pinfo, pinfo->ateTimestamp + pinfo->time_to_eat
			+ pinfo->time_to_sleep);
	}
	return (exit_philo_process(mystruct, pinfo, 0));
}
