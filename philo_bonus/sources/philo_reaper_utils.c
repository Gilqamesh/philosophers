/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_reaper_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:26:35 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 19:27:12 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static bool	philo_has_died(t_philosophers *mystruct, t_node_binary *cur,
long int next_timestamp)
{
	if (mystruct->ph_arr[
			((t_philo_eat_info *)cur->content)->philosopher_index]
		.ateTimestamp
		== ((t_philo_eat_info *)cur->content)->ateTimestamp)
	{
		sem_post(mystruct->semQueue);
		philo_print_status(((t_philo_eat_info *)cur->content)
			->philosopher_index + 1, PHILO_DIED, next_timestamp);
		philo_unlock_all_forks(mystruct);
		ft_nodbindel(cur);
		return (true);
	}
	return (false);
}

/*
** Returns true if a philosopher has not eaten in the queue for time_to_eat ms.
*/
bool	has_philo_died(t_philosophers *mystruct, t_node_binary *cur)
{
	long int	next_timestamp;
	long int	timestamp;

	timestamp = philo_get_current_timestamp();
	next_timestamp
		= ((t_philo_eat_info *)cur->content)->ateTimestamp
		+ mystruct->time_to_die * 1000;
	if (next_timestamp < timestamp)
	{
		philo_print_status(((t_philo_eat_info *)cur->content)
			->philosopher_index + 1, PHILO_DIED, next_timestamp);
		philo_unlock_all_forks(mystruct);
		ft_nodbindel(cur);
		return (true);
	}
	philo_sleep_until_timestamp(next_timestamp);
	sem_wait(mystruct->semQueue);
	if (philo_has_died(mystruct, cur, next_timestamp) == true)
		return (true);
	sem_post(mystruct->semQueue);
	ft_nodbindel(cur);
	return (false);
}
