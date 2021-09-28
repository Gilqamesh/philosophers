/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_reaper_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:26:35 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:24:44 by edavid           ###   ########.fr       */
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
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
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
	pthread_mutex_lock(&mystruct->time_left_lst_mutex);
	if (philo_has_died(mystruct, cur, next_timestamp) == true)
		return (true);
	pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
	ft_nodbindel(cur);
	return (false);
}

bool	is_game_over2(t_philosophers *mystruct)
{
	if (mystruct->game_over == true)
	{
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		return (true);
	}
	return (false);
}
