/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 13:23:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:26:27 by edavid           ###   ########.fr       */
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
	pthread_mutex_lock(pinfo->ref_queue);
	pinfo->ateTimestamp = timestamp;
	ft_fifonodbinenqueue(pinfo->meal_queue,
		ft_nodbinnew(philo_new_philo_info(pinfo->phNum - 1,
				pinfo->ateTimestamp)));
	pthread_mutex_unlock(pinfo->ref_queue);
}

/*
** Unlocks all forks that are currently locked.
*/
void	philo_unlock_all_forks(t_philosophers *mystruct)
{
	int	i;
	int	j;

	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < mystruct->phNum)
		{
			pthread_mutex_lock(&mystruct->forks[i].is_available_mutex);
			if (mystruct->forks[i].is_available == false)
			{
				pthread_mutex_unlock(&mystruct->forks[i].fork);
				mystruct->forks[i].is_available = true;
			}
			pthread_mutex_unlock(&mystruct->forks[i].is_available_mutex);
		}
		usleep(long_int_max_of(mystruct->time_to_die * 1000,
				mystruct->time_to_sleep * 1000));
	}
}
