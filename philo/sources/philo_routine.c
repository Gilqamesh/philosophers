/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:24:55 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 20:10:12 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

void	*reaper_routine(void *mystructPtr)
{
	t_philosophers	*mystruct;
	t_node_binary	*next_in_queue;
	int				i;
	long int		time_to_check_again;
	long int		timestamp;
	struct timeval	cur_time;

	mystruct = (t_philosophers *)mystructPtr;
	gettimeofday(&mystruct->start_time, NULL);
	time_to_check_again = philo_timval_to_microseconds(&mystruct->start_time);
	mystruct->first_in_queue = ft_nodbinnew(philo_new_philo_info(0, 0));
	ft_fifonodbinenqueue(&mystruct->meal_timestamps, mystruct->first_in_queue);
	i = 0;
	while (++i < mystruct->n_of_philosophers)
		ft_fifonodbinenqueue(&mystruct->meal_timestamps,
			ft_nodbinnew(philo_new_philo_info(i, 0)));
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		mystruct->array_of_philosophers[i].last_eat_timestamp = time_to_check_again;
		pthread_mutex_unlock(&mystruct->start_mutexes[i]);
	}
	while (1)
	{
		pthread_mutex_lock(&mystruct->time_left_lst_mutex);
		next_in_queue = ft_fifonodbindequeue(
			&mystruct->meal_timestamps,
			mystruct->first_in_queue);
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
		if (next_in_queue == NULL)
		{
			usleep(mystruct->time_to_die * 100);
			continue ;
		}
		mystruct->first_in_queue = next_in_queue->prev;
		if (mystruct->array_of_philosophers[((t_philo_eat_info *)next_in_queue->content)->philosopher_index]
			.last_eat_timestamp != ((t_philo_eat_info *)next_in_queue->content)->last_eat_timestamp)
		{
			// If the philosopher has already eaten, we can continue to the next
			// philosopher in queue.
			ft_nodbindel(next_in_queue);
			continue ;
		}
		gettimeofday(&cur_time, NULL);
		timestamp = philo_timval_to_microseconds(&cur_time);
		time_to_check_again = mystruct->time_to_die * 1000 - (timestamp
			- ((t_philo_eat_info *)next_in_queue->content)->last_eat_timestamp);
		if (time_to_check_again < 0)
		{
			philo_print_status(((t_philo_eat_info *)next_in_queue->content)
				->philosopher_index + 1, PHILO_DIED, timestamp);
			philo_unlock_all_forks(mystruct);
			ft_nodbindel(next_in_queue);
			break ;
		}
		philo_sleep_until_timestamp(timestamp + time_to_check_again);
		if (mystruct->array_of_philosophers[((t_philo_eat_info *)next_in_queue->content)->philosopher_index]
			.last_eat_timestamp == ((t_philo_eat_info *)next_in_queue->content)->last_eat_timestamp)
		{
			gettimeofday(&cur_time, NULL);
			philo_print_status(((t_philo_eat_info *)next_in_queue->content)
				->philosopher_index + 1, PHILO_DIED, timestamp
				+ time_to_check_again);
			philo_unlock_all_forks(mystruct);
			ft_nodbindel(next_in_queue);
			break ;
		}
		ft_nodbindel(next_in_queue);
	}
	return (NULL);
}

void	*philo_routine(void *info)
{
	t_philosopher_info	*pinfo;
	t_philosophers		*mystruct;
	struct timeval		cur_time;
	long int			timestamp;
	long int			time_left;
	bool				have_eaten;

	pinfo = (t_philosopher_info *)info;
	mystruct = philo_get_mystruct(NULL);
	pthread_mutex_lock(pinfo->reference_to_start_mutex);
	have_eaten = false;
	gettimeofday(&cur_time, NULL);
	if (pinfo->philosopher_number % 2 == 0)
	{
		pinfo->last_eat_timestamp = philo_timval_to_microseconds(&cur_time);
		pinfo->last_think_start_timestamp = pinfo->last_eat_timestamp;
		if (philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING,
			pinfo->last_eat_timestamp))
			return (NULL);
		philo_sleep_until_timestamp(philo_timval_to_microseconds(&cur_time)
			+ pinfo->time_to_sleep);
	}
	else
	{
		pinfo->last_think_start_timestamp
			= philo_timval_to_microseconds(&cur_time);
		if (philo_print_status(pinfo->philosopher_number, PHILO_IS_THINKING,
			pinfo->last_think_start_timestamp))
			return (NULL);
	}
	while (pinfo->number_of_meals_needed)
	{
		pthread_mutex_lock(&mystruct->game_over_mutex);
		if (mystruct->game_over == true)
			break ;
		pthread_mutex_unlock(&mystruct->game_over_mutex);
		if (have_eaten == true)
		{
			if (philo_print_status(pinfo->philosopher_number, PHILO_IS_THINKING,
				pinfo->last_think_start_timestamp))
				break ;
			have_eaten = false;
		}
		// First check for the left fork
		pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
		if (pinfo->reference_to_left_fork->is_available)
		{
			// Left fork is available
			pinfo->reference_to_left_fork->is_available = false;
			pthread_mutex_lock(&pinfo->reference_to_left_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			//
			gettimeofday(&cur_time, NULL);
			if (philo_print_status(pinfo->philosopher_number, PHILO_FORK,
				pinfo->last_think_start_timestamp))
				break ;
			// Wait until the right fork is available
			pthread_mutex_lock(&pinfo->reference_to_right_fork->fork);
			gettimeofday(&cur_time, NULL);
			if (philo_print_status(pinfo->philosopher_number, PHILO_FORK,
				philo_timval_to_microseconds(&cur_time)))
				break ;
			// Once it becomes available
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			// Start eating
			philo_enqueue(pinfo, &cur_time);
			if (philo_print_status(pinfo->philosopher_number, PHILO_IS_EATING,
				pinfo->last_eat_timestamp))
				break ;
			have_eaten = true;
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			philo_sleep_until_timestamp(pinfo->last_eat_timestamp
				+ pinfo->time_to_eat);
			// Stop eating
			//
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			//
			if (philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING,
				pinfo->last_eat_timestamp + pinfo->time_to_eat))
				break ;
			philo_sleep_until_timestamp(pinfo->last_eat_timestamp
				+ pinfo->time_to_eat + pinfo->time_to_sleep);
			pinfo->last_think_start_timestamp = pinfo->last_eat_timestamp
				+ pinfo->time_to_eat + pinfo->time_to_sleep;
			continue ;
		}
		else
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
		pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
		if (pinfo->reference_to_right_fork->is_available)
		{
			// Right fork is available
			pinfo->reference_to_right_fork->is_available = false;
			pthread_mutex_lock(&pinfo->reference_to_right_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			//
			if (philo_print_status(pinfo->philosopher_number, PHILO_FORK,
				pinfo->last_think_start_timestamp))
				break ;
			// Wait until the left fork is available
			pthread_mutex_lock(&pinfo->reference_to_left_fork->fork);
			gettimeofday(&cur_time, NULL);
			if (philo_print_status(pinfo->philosopher_number, PHILO_FORK,
				philo_timval_to_microseconds(&cur_time)))
				break ;
			// Once it becomes available
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			// Start eating]
			philo_enqueue(pinfo, &cur_time);
			if (philo_print_status(pinfo->philosopher_number, PHILO_IS_EATING,
				pinfo->last_eat_timestamp))
				break ;
			have_eaten = true;
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			philo_sleep_until_timestamp(pinfo->last_eat_timestamp
				+ pinfo->time_to_eat);
			// Stop eating
			//
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->fork);
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			//
			if (philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING,
				pinfo->last_eat_timestamp + pinfo->time_to_eat))
				break ;
			philo_sleep_until_timestamp(pinfo->last_eat_timestamp
				+ pinfo->time_to_eat + pinfo->time_to_sleep);
			pinfo->last_think_start_timestamp = pinfo->last_eat_timestamp
				+ pinfo->time_to_eat + pinfo->time_to_sleep;
			continue ;
		}
		else
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
		// If haven't found any available fork, wait for X amount of time
		// before retrying.
		gettimeofday(&cur_time, NULL);
		timestamp = philo_timval_to_microseconds(&cur_time);
		time_left = pinfo->time_to_die - (timestamp
			- pinfo->last_eat_timestamp);
		// Wait for time_left till starvation / 10.
		usleep(time_left / 10);
		have_eaten = false;
	}
	// printf("Philosopher number %d is done\n", pinfo->philosopher_number);
	return (NULL);
}
