/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_reaper_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:13:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 21:11:33 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

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
		sem_post(mystruct->semQueue);
		ft_nodbindel(cur);
		return (true);
	}
	return (false);
}

static bool	noone_in_queue(t_philosophers *mystruct, t_node_binary *cur)
{
	if (cur == NULL)
	{
		mystruct->first_in_queue = NULL;
		sem_post(mystruct->semQueue);
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
	while (1)
	{
		sem_wait(mystruct->semQueue);
		next_ph = ft_fifonodbindequeue(&mystruct->meal_queue,
				mystruct->first_in_queue);
		if (noone_in_queue(mystruct, next_ph) == true)
			continue ;
		if (philo_is_not_hungry(mystruct, next_ph) == true)
			continue ;
		sem_post(mystruct->semQueue);
		if (has_philo_died(mystruct, next_ph) == true)
		{
			sem_post(mystruct->semFinish);
			break ;
		}
	}
	return (NULL);
}
