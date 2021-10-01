/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:55:32 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 15:29:46 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

// TODO: Measure time this function takes in the whole process.
/*
** Takes in as parameter in microsececonds a timestamp in milliseconds.
*/
void	philo_sleep_until_timestamp(long int timestamp)
{
	static bool				first_time = true;
	static t_philosophers	*mystruct;

	if (first_time == true)
	{
		first_time = false;
		mystruct = philo_get_mystruct(NULL);
	}
	while (1)
	{
		if (timestamp <= philo_get_current_timestamp())
			return ;
		if (mystruct->phNum < 10)
			usleep(10);
		else if (mystruct->phNum < 100)
			usleep(100);
		else if (mystruct->phNum < 150)
			usleep(1500);
		else
			usleep(2000);
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
