/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 18:38:39 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 20:06:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

void	philo_fork(t_philosophers *mystruct, t_philosopher_info *pinfo)
{
	sem_wait(mystruct->forks);
	philo_print_status(pinfo->phNum, PHILO_FORK, philo_get_current_timestamp());
	sem_wait(mystruct->forks);
	pinfo->ateTimestamp = philo_get_current_timestamp();
	philo_print_status(pinfo->phNum, PHILO_FORK, pinfo->ateTimestamp);
}

void	philo_eat(t_philosophers *mystruct,
t_philosopher_info *pinfo, long int timestamp)
{
	philo_enqueue(pinfo, timestamp);
	if (pinfo->nOfMeals != CANT_STOP_EATING)
	{
		pinfo->nOfMeals--;
		if (pinfo->nOfMeals == 0)
			pinfo->stopDeath = true;
	}
	philo_print_status(pinfo->phNum, PHEAT, timestamp);
	philo_sleep_until_timestamp(timestamp + pinfo->time_to_eat);
	sem_post(mystruct->forks);
	sem_post(mystruct->forks);
}

void	philo_sleep(t_philosopher_info *pinfo,
long int timestamp)
{
	philo_print_status(pinfo->phNum, PHSL, timestamp);
	philo_sleep_until_timestamp(timestamp + pinfo->time_to_sleep);
	pinfo->lastThinkTimeStamp = timestamp + pinfo->time_to_sleep;
}

void	philo_think(t_philosopher_info *pinfo,
long int timestamp)
{
	philo_print_status(pinfo->phNum, PHTH, timestamp);
	philo_sleep_until_timestamp(timestamp + pinfo->time_to_think);
}
