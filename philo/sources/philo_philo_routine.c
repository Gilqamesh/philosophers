/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philo_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:24:55 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 12:25:03 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

// This line was:
// pinfo->lastThinkTimeStamp += pinfo->time_to_sleep;
// Make sure that the below code doesnt break the program!
// pinfo->lastThinkTimeStamp += pinfo->time_to_eat;
/*
** 1. Waits until the start_mutexes are unlocked.
** 2. Sleeps until the start time.
** 3. Initialize every second philosopher to start sleeping.
*/
static int	initialize_philo(t_philosopher_info *pinfo)
{
	pthread_mutex_lock(pinfo->refStartMutex);
	pinfo->lastThinkTimeStamp = pinfo->ateTimestamp;
	philo_sleep_until_timestamp(pinfo->ateTimestamp);
	if (pinfo->phNum % 2 == 0)
	{
		pinfo->lastThinkTimeStamp += pinfo->time_to_eat;
		if (philo_print_status(pinfo->phNum, PHSL, pinfo->ateTimestamp))
			return (1);
		philo_sleep_until_timestamp(pinfo->lastThinkTimeStamp);
	}
	else if (philo_print_status(pinfo->phNum, PHTH, pinfo->lastThinkTimeStamp))
		return (1);
	return (0);
}

static bool	is_game_over(t_philosophers *mystruct)
{
	pthread_mutex_lock(&mystruct->game_over_mutex);
	if (mystruct->game_over == true)
	{
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		return (true);
	}
	return (false);
}

static void	unlock_both_forks(t_philosopher_info *pinfo)
{
	pthread_mutex_unlock(&pinfo->refRightFork->fork);
	pthread_mutex_unlock(&pinfo->refLeftFork->fork);
}

/*
** First check for left fork.
** Returns 1 if successfully eaten.
** Returns -1 if game is over.
** Returns 0 if both forks are occupied currently.
*/
static int	try_to_eat(t_philosopher_info *pinfo)
{
	long int	timestamp;

	if (try_left_fork(pinfo) == true)
		return (-1);
	pthread_mutex_lock(&pinfo->refRightFork->fork);
	timestamp = philo_get_current_timestamp();
	if (try_right_fork(pinfo, timestamp) == true
		|| try_eating(pinfo, timestamp) == true)
		return (-1);
	unlock_both_forks(pinfo);
	if (philo_print_status(pinfo->phNum, PHSL,
			pinfo->ateTimestamp + pinfo->time_to_eat))
		return (-1);
	philo_sleep_until_timestamp(pinfo->ateTimestamp
		+ pinfo->time_to_eat + pinfo->time_to_sleep);
	pinfo->lastThinkTimeStamp = pinfo->ateTimestamp
		+ pinfo->time_to_eat + pinfo->time_to_sleep;
	if (philo_print_status(pinfo->phNum, PHTH, pinfo->lastThinkTimeStamp))
		return (-1);
	return (1);
}

void	*philo_routine(void *info)
{
	t_philosopher_info	*pinfo;
	t_philosophers		*mystruct;

	pinfo = (t_philosopher_info *)info;
	mystruct = philo_get_mystruct(NULL);
	if (initialize_philo(pinfo))
		return (NULL);
	while (pinfo->nOfMeals)
	{
		if (is_game_over(mystruct) == true)
			break ;
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		if (try_to_eat(pinfo) < 0)
			break ;
	}
	return (NULL);
}
