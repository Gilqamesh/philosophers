/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 13:23:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/29 20:09:35 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Returns the number if valid, -1 otherwise.
** Can only contain numeric characters.
** Can only have a single '-' or '+' as a prefix.
*/
bool	philo_is_valid_positive_number(char *str)
{
	char	*tmp;

	tmp = str;
	if (str == NULL)
		return (false);
	if (*str == '-')
		return (false);
	if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	if (philo_atoi(tmp) == INT_MAX && ft_strcmp(tmp, "2147483647"))
		return (false);
	return (true);
}

/*
** This function is called before eating.
** Updates last meal timestamp for 'pinfo'.
** Updates the queue of timestamps.
*/
void	philo_enqueue(t_philosopher_info *pinfo, long int timestamp)
{
	static bool	first_called = true;
	static t_philosophers *mystruct;

	if (first_called == true)
	{
		first_called = false;
		mystruct = philo_get_mystruct(NULL);
	}
	sem_wait(mystruct->semQueue);
	pinfo->ateTimestamp = timestamp;
	ft_fifonodbinenqueue(pinfo->meal_queue,
		ft_nodbinnew(philo_new_philo_info(pinfo->phNum - 1,
				pinfo->ateTimestamp)));
	sem_post(mystruct->semQueue);
}

sem_t	*philo_sem_init(char *name, unsigned int value)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, 0777, value));
}

/*
** Unlocks all forks that are currently locked.
*/
void	philo_unlock_all_forks(t_philosophers *mystruct)
{
	sem_post(mystruct->forks);
	sem_post(mystruct->forks);
}
