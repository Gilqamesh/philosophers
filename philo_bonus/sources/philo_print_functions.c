/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 18:13:08 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 20:25:49 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static int	print_status(enum e_philosopher_status philo_status,
long int timestamp, int phNum)
{
	if (philo_status == PHEAT)
		printf("%ld %d is eating\n", timestamp, phNum);
	else if (philo_status == PHSL)
		printf("%ld %d is sleeping\n", timestamp, phNum);
	else if (philo_status == PHTH)
		printf("%ld %d is thinking\n", timestamp, phNum);
	else if (philo_status == PHILO_FORK)
		printf("%ld %d has taken a fork\n", timestamp, phNum);
	else if (philo_status == PHILO_DIED)
	{
		printf("%ld %d died\n", timestamp, phNum);
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
		start_timestamp = mystruct->startTime;
	}
	sem_wait(mystruct->semPrint);
	timestamp = (timestamp - start_timestamp) / 1000;
	if (print_status(philo_status, timestamp, phNum))
		return (1);
	sem_post(mystruct->semPrint);
	return (0);
}
