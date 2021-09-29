/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:55:32 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 20:49:40 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

// TODO: Measure time this function takes in the whole process.
/*
** Takes in as parameter in microsececonds a timestamp in milliseconds.
*/
void	philo_sleep_until_timestamp(long int timestamp)
{
	while (1)
	{
		if (timestamp <= philo_get_current_timestamp())
			return ;
		usleep(100);
	}
}

/*
** Returns the current timestamp in microseconds compared to start.
*/
long int	philo_get_current_timestamp(void)
{
	struct timeval	cur;

	gettimeofday(&cur, NULL);
	return (philo_timval_to_microseconds(&cur));
}

/*
** Returns in microseconds the sum value in 't'.
*/
long int	philo_timval_to_microseconds(struct timeval *t)
{
	if (t == NULL)
		return (-1);
	return (t->tv_sec * 1000000 + t->tv_usec);
}
