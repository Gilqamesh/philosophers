/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 18:13:08 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 20:06:06 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Returns 1 if game is over. 0 otherwise.
*/
int	philo_print_status(int philosopher_number,
enum e_philosopher_status philo_status, long int timestamp)
{
	static t_philosophers	*mystruct;
	static long int			start_timestamp;
	static bool				first_called = true;

	if (first_called == true)
	{
		first_called = false;
		mystruct = philo_get_mystruct(NULL);
		start_timestamp = philo_timval_to_microseconds(&mystruct->start_time);
	}
	pthread_mutex_lock(&mystruct->print_mutex);
	if (philo_is_game_over(mystruct) == true)
	{
		pthread_mutex_unlock(&mystruct->print_mutex);
		return (1);
	}
	timestamp = (timestamp - start_timestamp) / 1000;
	if (philo_status == PHILO_IS_EATING)
		printf("%ld %d is eating\n", timestamp, philosopher_number);
	else if (philo_status == PHILO_IS_SLEEPING)
		printf("%ld %d is sleeping\n", timestamp, philosopher_number);
	else if (philo_status == PHILO_IS_THINKING)
		printf("%ld %d is thinking\n", timestamp, philosopher_number);
	else if (philo_status == PHILO_FORK)
		printf("%ld %d has taken a fork\n", timestamp, philosopher_number);
	else if (philo_status == PHILO_FORK2)
	{
		printf("%ld %d has taken a fork\n", timestamp, philosopher_number);
		printf("%ld %d has taken a fork\n", timestamp, philosopher_number);
	}
	else if (philo_status == PHILO_DIED)
	{
		pthread_mutex_lock(&mystruct->game_over_mutex);
		mystruct->game_over = true;
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		printf("%ld %d died\n", timestamp, philosopher_number);
		pthread_mutex_unlock(&mystruct->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&mystruct->print_mutex);
	return (0);
}
