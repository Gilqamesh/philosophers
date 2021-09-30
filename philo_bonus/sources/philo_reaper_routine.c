/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_reaper_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 18:13:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 20:07:04 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

static bool	has_philo_died(t_philosophers *mystruct, t_philosopher_info *pinfo,
t_node_binary *cur)
{
	if (cur == NULL)
	{
		if (pinfo->stopDeath == true)
			return (true);
		philo_print_status(pinfo->phNum, PHILO_DIED, pinfo->ateTimestamp
			+ pinfo->time_to_die);
		sem_post(mystruct->semFinish);
		return (true);
	}
	pinfo->first_in_queue = cur->prev;
	return (false);
}

void	*reaper_routine(void *pinfoPtr)
{
	t_philosopher_info	*pinfo;
	t_philosophers		*mystruct;
	t_node_binary		*lastAte;
	long int			next_timestamp;

	pinfo = (t_philosopher_info *)pinfoPtr;
	mystruct = philo_get_mystruct(NULL);
	while (1)
	{
		sem_wait(pinfo->semQueue);
		lastAte = ft_fifonodbindequeue(&pinfo->meal_queue,
			pinfo->first_in_queue);
		if (has_philo_died(mystruct, pinfo, lastAte) == true)
			break ;
		sem_post(pinfo->semQueue);
		next_timestamp = *(long int *)lastAte->content + pinfo->time_to_die;
		ft_nodbindel(lastAte);
		philo_sleep_until_timestamp(next_timestamp);
	}
	return (NULL);
}
