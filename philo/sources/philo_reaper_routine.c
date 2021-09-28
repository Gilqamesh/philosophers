/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_reaper_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:13:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:25:01 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** 1. Initializes the queue with the starting timestamp of philosophers + delay.
** 2. Waits till the timestamp.
** 3. Unlocks the philosopher threads.
*/
static void	init_queue_and_start_philos(t_philosophers *mystruct)
{
	long int	ateTimestamp;
	int			i;

	gettimeofday(&mystruct->start_time, NULL);
	ateTimestamp = philo_timval_to_microseconds(&mystruct->start_time)
		+ INIT_TIME_IN_MICROSECONDS;
	philo_print_status(0, 0, 0);
	mystruct->first_in_queue = ft_nodbinnew(philo_new_philo_info(0,
				ateTimestamp));
	ft_fifonodbinenqueue(&mystruct->meal_queue, mystruct->first_in_queue);
	mystruct->ph_arr[0].ateTimestamp = ateTimestamp;
	i = 0;
	while (++i < mystruct->phNum)
	{
		ft_fifonodbinenqueue(&mystruct->meal_queue,
			ft_nodbinnew(philo_new_philo_info(i, ateTimestamp)));
		mystruct->ph_arr[i].ateTimestamp = ateTimestamp;
	}
	i = -1;
	while (++i < mystruct->phNum)
		pthread_mutex_unlock(&mystruct->start_mutexes[i]);
}

/*
** If the philosopher has eaten in the meantime.
** OR
** The philosopher does not need to eat anymore.
*/
static bool	philo_is_not_hungry(t_philosophers *mystruct, t_node_binary *cur)
{
	if (mystruct->ph_arr[((t_philo_eat_info *)cur->content)->philosopher_index]
		.ateTimestamp != ((t_philo_eat_info *)cur->content)->ateTimestamp
		|| mystruct->ph_arr[
			((t_philo_eat_info *)cur->content)->philosopher_index]
		.nOfMeals == 0)
	{
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
		ft_nodbindel(cur);
		return (true);
	}
	return (false);
}

static bool	noone_in_queue(t_philosophers *mystruct, t_node_binary *cur)
{
	if (cur == NULL)
	{
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
		mystruct->first_in_queue = NULL;
		usleep(mystruct->time_to_die * 100);
		return (true);
	}
	else
		mystruct->first_in_queue = cur->prev;
	return (false);
}

void	*reaper_routine(void *mystructPtr)
{
	t_philosophers	*mystruct;
	t_node_binary	*next_ph;

	mystruct = (t_philosophers *)mystructPtr;
	init_queue_and_start_philos(mystruct);
	while (1)
	{
		pthread_mutex_lock(&mystruct->game_over_mutex);
		if (is_game_over2(mystruct) == true)
			break ;
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		pthread_mutex_lock(&mystruct->time_left_lst_mutex);
		next_ph = ft_fifonodbindequeue(&mystruct->meal_queue,
				mystruct->first_in_queue);
		if (noone_in_queue(mystruct, next_ph) == true)
			continue ;
		if (philo_is_not_hungry(mystruct, next_ph) == true)
			continue ;
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
		if (has_philo_died(mystruct, next_ph) == true)
			break ;
	}
	return (NULL);
}
