/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 18:13:08 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:09:34 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static int	print_status(t_philosophers *mystruct,
enum e_philosopher_status philo_status, long int timestamp,
int phNum)
{
	if (philo_status == PHEAT)
		printf("%ld %d is eating\n", timestamp, phNum);
	else if (philo_status == PHSL)
		printf("%ld %d is sleeping\n", timestamp, phNum);
	else if (philo_status == PHTH)
		printf("%ld %d is thinking\n", timestamp, phNum);
	else if (philo_status == PHILO_FORK)
		printf("%ld %d has taken a fork\n", timestamp, phNum);
	else if (philo_status == PHILO_FORK2)
	{
		printf("%ld %d has taken a fork\n", timestamp, phNum);
		printf("%ld %d has taken a fork\n", timestamp, phNum);
	}
	else if (philo_status == PHILO_DIED)
	{
		pthread_mutex_lock(&mystruct->game_over_mutex);
		mystruct->game_over = true;
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		printf("%ld %d died\n", timestamp, phNum);
		pthread_mutex_unlock(&mystruct->print_mutex);
		return (1);
	}
	return (0);
}

/*
** Returns 1 if game is over. 0 otherwise.
*/
int	philo_print_status(int phNum,
enum e_philosopher_status philo_status, long int timestamp)
{
	static t_philosophers	*mystruct;
	static long int			start_timestamp;
	static bool				first_called = true;

	if (first_called == true)
	{
		first_called = false;
		mystruct = philo_get_mystruct(NULL);
		start_timestamp = philo_timval_to_microseconds(&mystruct->start_time)
			+ INIT_TIME_IN_MICROSECONDS;
		return (0);
	}
	pthread_mutex_lock(&mystruct->print_mutex);
	pthread_mutex_lock(&mystruct->game_over_mutex);
	if (mystruct->game_over == true)
	{
		pthread_mutex_unlock(&mystruct->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&mystruct->game_over_mutex);
	timestamp = (timestamp - start_timestamp) / 1000;
	if (print_status(mystruct, philo_status, timestamp, phNum))
		return (1);
	pthread_mutex_unlock(&mystruct->print_mutex);
	return (0);
}
