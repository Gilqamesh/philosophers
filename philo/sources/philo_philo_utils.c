/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philo_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:45:34 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 12:28:45 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

bool	try_right_fork(t_philosopher_info *pinfo, long int timestamp)
{
	if (philo_print_status(pinfo->phNum, PHILO_FORK, timestamp))
		return (true);
	return (false);
}

bool	try_left_fork(t_philosopher_info *pinfo)
{
	pthread_mutex_lock(&pinfo->refLeftFork->fork);
	if (philo_print_status(pinfo->phNum, PHILO_FORK,
			philo_get_current_timestamp()))
		return (true);
	return (false);
}

bool	try_eating(t_philosopher_info *pinfo, long int timestamp)
{
	philo_enqueue(pinfo, timestamp);
	if (philo_print_status(pinfo->phNum, PHEAT, pinfo->ateTimestamp))
		return (true);
	if (pinfo->nOfMeals != CANT_STOP_EATING)
		pinfo->nOfMeals--;
	philo_sleep_until_timestamp(pinfo->ateTimestamp + pinfo->time_to_eat);
	return (false);
}
