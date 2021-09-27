/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 18:13:08 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 17:22:31 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Returns 1 if game is over. 0 otherwise.
*/
int	philo_print_status(int philosopher_number,
enum e_philosopher_status philo_status, struct timeval *cur_time)
{
	static t_philosophers	*mystruct;
	long int				timestamp;

	if (mystruct == NULL)
		mystruct = philo_get_mystruct(NULL);
	if (philo_is_game_over(mystruct) == true)
	{
		printf("Hello from philosopher number: (%d)\n", philosopher_number);
		return (1);
	}
	pthread_mutex_lock(&mystruct->print_mutex);
	timestamp = philo_calc_microseconds_difference(cur_time,
		&mystruct->start_time) / 1000;
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
