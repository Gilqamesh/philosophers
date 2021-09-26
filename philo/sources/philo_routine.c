/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 14:24:55 by edavid            #+#    #+#             */
/*   Updated: 2021/09/26 19:59:07 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

void	*reaper_routine(void *mystructPtr)
{
	t_philosophers	*mystruct;
	t_node_binary	*next_in_queue;
	int				i;
	long int		tmp;
	long int		timestamp;
	struct timeval	cur_time;

	mystruct = (t_philosophers *)mystructPtr;
	gettimeofday(&mystruct->start_time, NULL);
	tmp = mystruct->start_time.tv_sec * 1000000 + mystruct->start_time.tv_usec;
	i = -1;
	while (++i < mystruct->n_of_philosophers)
	{
		mystruct->array_of_philosophers[i].start_timestamp_in_microseconds = tmp;
		pthread_mutex_unlock(&mystruct->start_mutexes[i]);
	}
	while (1)
	{
		pthread_mutex_lock(&mystruct->time_left_lst_mutex);
		next_in_queue = ft_fifonodbindequeue(
			&mystruct->time_left_till_starvation_lst,
			mystruct->first_in_queue);
		if (next_in_queue == NULL)
		{
			pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
			usleep(mystruct->time_to_die * 500);
			continue ;
		}
		// printf("After dequeue:\n");
		// ft_nodbinprint(mystruct->time_left_till_starvation_lst);
		mystruct->first_in_queue = next_in_queue->prev;
		if (mystruct->array_of_philosophers[((t_philo_eat_info *)next_in_queue->content)->philosopher_index]
			.last_finished_eating_timestamp != ((t_philo_eat_info *)next_in_queue->content)->finished_eating_at_timestamp)
		{
			// If the philosopher has already eaten, we can continue to the next
			// philosopher in queue.
			pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
			ft_nodbindel(next_in_queue);
			continue ;
		}
		gettimeofday(&cur_time, NULL);
		timestamp = philo_calc_microseconds_difference(&cur_time, &mystruct->start_time);
		tmp = mystruct->time_to_die * 1000 - (timestamp
			- ((t_philo_eat_info *)next_in_queue->content)->finished_eating_at_timestamp);
		pthread_mutex_unlock(&mystruct->time_left_lst_mutex);
		if (tmp < 0)
		{
			pthread_mutex_lock(&mystruct->time_left_lst_mutex);
			philo_print_status(((t_philo_eat_info *)next_in_queue->content)
				->philosopher_index + 1, PHILO_DIED);
			ft_nodbindel(next_in_queue);
			break ;
		}
		usleep(tmp);
		if (mystruct->array_of_philosophers[((t_philo_eat_info *)next_in_queue->content)->philosopher_index]
			.last_finished_eating_timestamp == ((t_philo_eat_info *)next_in_queue->content)->finished_eating_at_timestamp)
		{
			pthread_mutex_lock(&mystruct->time_left_lst_mutex);
			philo_print_status(((t_philo_eat_info *)next_in_queue->content)
				->philosopher_index + 1, PHILO_DIED);
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
	struct timeval		cur_time;
	long int			timestamp;
	long int			time_left;

	pinfo = (t_philosopher_info *)info;
	pthread_mutex_lock(pinfo->reference_to_start_mutex);
	while (pinfo->number_of_meals_needed)
	{
		philo_print_status(pinfo->philosopher_number, PHILO_IS_THINKING);
		pthread_mutex_lock(&pinfo->set_of_forks.is_available_mutex);
		if (pinfo->set_of_forks.is_available)
		{
			// If both forks are available
			pinfo->set_of_forks.is_available = false;
			pthread_mutex_unlock(&pinfo->set_of_forks.is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_left_fork->is_available_mutex);
			pinfo->set_of_forks.reference_to_left_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_left_fork->is_available_mutex);
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_right_fork->is_available_mutex);
			pinfo->set_of_forks.reference_to_right_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_right_fork->is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_left_fork->fork);
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_right_fork->fork);
			//
			// Eat
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			philo_print_status(pinfo->philosopher_number, PHILO_IS_EATING);
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			usleep(pinfo->time_to_eat);
			// Stop eating
			philo_enqueue(pinfo);
			//
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_left_fork->is_available_mutex);
			pinfo->set_of_forks.reference_to_left_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_left_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_left_fork->fork);
			//
			pthread_mutex_lock(&pinfo->set_of_forks.reference_to_right_fork->is_available_mutex);
			pinfo->set_of_forks.reference_to_right_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_right_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->set_of_forks.reference_to_right_fork->fork);
			//
			pthread_mutex_lock(&pinfo->set_of_forks.is_available_mutex);
			pinfo->set_of_forks.is_available = true;
			pthread_mutex_unlock(&pinfo->set_of_forks.is_available_mutex);
			// Sleep
			philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING);
			usleep(pinfo->time_to_sleep);
			continue ;
		}
		else
			pthread_mutex_unlock(&pinfo->set_of_forks.is_available_mutex);
		// Both of the forks aren't available
		// First check for the left fork
		pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
		if (pinfo->reference_to_left_fork->is_available)
		{
			// Left fork is available
			pinfo->reference_to_left_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->reference_to_left_fork->fork);
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			// Wait until the right fork is available
			pthread_mutex_lock(&pinfo->reference_to_right_fork->fork);
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			// Once it becomes available
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			// Start eating
			philo_print_status(pinfo->philosopher_number, PHILO_IS_EATING);
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			usleep(pinfo->time_to_eat);
			// Stop eating
			philo_enqueue(pinfo);
			//
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->fork);
			//
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->fork);
			//
			philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING);
			usleep(pinfo->time_to_sleep);
			continue ;
		}
		else
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
		pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
		if (pinfo->reference_to_left_fork->is_available)
		{
			// Right fork is available
			pinfo->reference_to_right_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			//
			pthread_mutex_lock(&pinfo->reference_to_right_fork->fork);
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			// Wait until the left fork is available
			pthread_mutex_lock(&pinfo->reference_to_left_fork->fork);
			philo_print_status(pinfo->philosopher_number, PHILO_FORK);
			// Once it becomes available
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = false;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			// Start eating
			philo_print_status(pinfo->philosopher_number, PHILO_IS_EATING);
			if (pinfo->number_of_meals_needed != CANT_STOP_EATING)
				pinfo->number_of_meals_needed--;
			usleep(pinfo->time_to_eat);
			// Stop eating
			philo_enqueue(pinfo);
			//
			pthread_mutex_lock(&pinfo->reference_to_left_fork->is_available_mutex);
			pinfo->reference_to_left_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->reference_to_left_fork->fork);
			//
			pthread_mutex_lock(&pinfo->reference_to_right_fork->is_available_mutex);
			pinfo->reference_to_right_fork->is_available = true;
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->fork);
			//
			philo_print_status(pinfo->philosopher_number, PHILO_IS_SLEEPING);
			usleep(pinfo->time_to_sleep);
			continue ;
		}
		else
			pthread_mutex_unlock(&pinfo->reference_to_right_fork->is_available_mutex);
		// If haven't found any available fork, wait for X amount of time
		// before retrying.
		gettimeofday(&cur_time, NULL);
		timestamp = philo_calc_microseconds_difference(&cur_time,
				pinfo->reference_to_start_time);
		time_left = pinfo->time_to_die - (timestamp
			- pinfo->last_finished_eating_timestamp);
		// Wait for time_left till starvation / 10.
		usleep(time_left / 100);
	}
	// printf("Philosopher number %d is done\n", pinfo->philosopher_number);
	return (NULL);
}
