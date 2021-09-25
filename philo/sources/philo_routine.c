/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:24:55 by edavid            #+#    #+#             */
/*   Updated: 2021/09/25 23:02:27 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

void	*reaper_routine(void *mystructPtr)
{
	t_philosophers	*mystruct;
	int				hungriest_index;
	int				i;
	int				tmp;
	struct timeval	cur_time;

	mystruct = (t_philosophers *)mystructPtr;
	gettimeofday(&mystruct->start_time, NULL);
	i = -1;
	while (++i < mystruct->n_of_philosophers)
		pthread_mutex_unlock(&mystruct->start_mutexes[i]);
	while (1)
	{
		pthread_mutex_lock(&mystruct->time_left_array_mutex);
		hungriest_index = 0;
		i = 0;
		while (++i < mystruct->n_of_philosophers)
			if (mystruct->time_left_till_starvation_array[i]
				< mystruct->time_left_till_starvation_array[hungriest_index])
				hungriest_index = i;
		tmp = mystruct->time_left_till_starvation_array[hungriest_index];
		mystruct->time_left_till_starvation_array[hungriest_index] = 0;
		pthread_mutex_unlock(&mystruct->time_left_array_mutex);
		usleep(tmp);
		if (mystruct->time_left_till_starvation_array[hungriest_index] == 0)
		{
			gettimeofday(&cur_time, NULL);
			printf("%d %d died\n", philo_calc_ms_difference(&cur_time,
				&mystruct->start_time), i + 1);
			break ;
		}
	}
	return (NULL);
}

void	*philo_routine(void *info)
{
	t_philosopher_info	*pinfo;
	struct timeval		cur_time;
	bool				is_set_of_forks_available;
	int					time_diff;

	pinfo = (t_philosopher_info *)info;
	pthread_mutex_lock(pinfo->reference_to_start_mutex);
	while (pinfo->number_of_meals_needed)
	{
		gettimeofday(&cur_time, NULL);
		printf("%d %d is thinking\n", philo_calc_ms_difference(&cur_time,
			pinfo->reference_to_start_time), pinfo->philosopher_number);
		pthread_mutex_lock(&pinfo->reference_to_set_of_forks->is_available_mutex);
		is_set_of_forks_available = pinfo->reference_to_set_of_forks->is_available;
		if (is_set_of_forks_available)
		{
			// If both forks are available
			pinfo->reference_to_set_of_forks->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_set_of_forks->is_available_mutex);
			pthread_mutex_lock(&pinfo->reference_to_set_of_forks->fork);
			// Eat
			gettimeofday(&cur_time, NULL);
			time_diff = philo_calc_ms_difference(&cur_time,
				pinfo->reference_to_start_time);
			printf("%d %d has taken a fork\n", time_diff, pinfo->philosopher_number);
			printf("%d %d is eating\n", time_diff, pinfo->philosopher_number);
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			usleep(pinfo->time_to_eat);
			pthread_mutex_lock(pinfo->reference_to_time_left_array_mutex);
			*pinfo->reference_to_time_left_array_position = pinfo->time_to_die;
			pthread_mutex_unlock(pinfo->reference_to_time_left_array_mutex);
			pthread_mutex_lock(&pinfo->reference_to_set_of_forks->is_available_mutex);
			pinfo->reference_to_set_of_forks->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_set_of_forks->is_available_mutex);
			pthread_mutex_unlock(&pinfo->reference_to_set_of_forks->fork);
			gettimeofday(&cur_time, NULL);
			printf("%d %d is sleeping\n", philo_calc_ms_difference(
				&cur_time, pinfo->reference_to_start_time),
				pinfo->philosopher_number);
			usleep(pinfo->time_to_sleep);
			continue ;
		}
		else
		{
			pthread_mutex_unlock(&pinfo->reference_to_set_of_forks->is_available_mutex);
			continue ;
		}
		// If haven't found any available fork, wait for X amount of time
		// before retrying.
		// Time_to_eat amount of time makes sense if both the philosopher's
		// neighbour just started eating.
		usleep(pinfo->time_to_eat);
	}
	printf("Philosopher number %d is done\n", pinfo->philosopher_number);
	return (NULL);
}
