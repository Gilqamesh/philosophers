/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 13:23:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/26 21:01:35 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Frees 'pointer' and sets it back to NULL.
** Returns 1.
*/
int	philo_my_free(void **pointer)
{
	if (pointer == NULL)
		return (1);
	if (*pointer)
		free(*pointer);
	*pointer = NULL;
	return (1);
}

/*
** Converts 'str' into an int.
** Operates under the assumption that 'str' is syntax checked.
** Only one '-' or '+' can be prefixes in 'str'.
*/
int	philo_atoi(char *str)
{
	int	result;
	int	multiplier;

	if (str == NULL)
		return (0);
	result = 0;
	multiplier = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			multiplier = -1;
	while (*str)
	{
		if (result > INT_MAX / 10)
		{
			if (multiplier == -1)
				return (INT_MIN);
			return (INT_MAX);
		}
		result = result * 10 + *str++ - '0';
	}
	return (multiplier * result);
}

/*
** Returns true if 'str' represents a valid positive number, false otherwise.
** Can only contain numeric characters.
** Can only have a single '-' or '+' as a prefix.
*/
bool	philo_is_valid_positive_number(char *str)
{
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
	return (true);
}

/*
** Calculates the difference in microseconds between 'current' and 'previous'.
*/
long int	philo_calc_microseconds_difference(struct timeval *current,
struct timeval *previous)
{
	return ((current->tv_sec - previous->tv_sec) * 1000000 + current->tv_usec
		- previous->tv_usec);
}

/*
** Allocates 'number_of_elements' objects, each of them 'size_of_each_element'
** in bytes.
** Sets all the allocates bytes to 0.
** Returns the allocated pointer on success, NULL on failure.
*/
void	*philo_calloc(int number_of_elements, int size_of_each_element)
{
	void	*ret;

	ret = malloc(number_of_elements * size_of_each_element);
	if (ret == NULL)
		return (NULL);
	memset(ret, 0, number_of_elements * size_of_each_element);
	return (ret);
}

/*
** Allocates and returns to 'ret' the return value of
** philo_calloc(n_of_els, size_of_el).
** Returns 0 on succcess, 1 on failure.
*/
int	philo_try_calloc(void **ret, int n_of_els, int size_of_el)
{
	if (ret == NULL)
		return (1);
	*ret = philo_calloc(n_of_els, size_of_el);
	if (*ret == NULL)
		return (1);
	return (0);
}

/*
** Allocates and returns to 'ret' the return value of malloc(size).
** Returns 0 on success, 1 on failure.
*/
int	philo_try_malloc(void **ret, int size)
{
	if (ret == NULL)
		return (1);
	*ret = malloc(size);
	if (*ret == NULL)
		return (1);
	return (0);
}

/*
** This function updates the queue ADT.
** Returns the hungriest philosopher's timestamp.
*/
// int	philo_update_queue_of_hungry_philosophers(t_node_binary *node,
// int new_timestamp)
// {
// 	return ();
// }

/*
** Returns in microseconds the sum value in 't'.
*/
long int	philo_timval_to_microseconds(struct timeval *t)
{
	if (t == NULL)
		return (-1);
	return (t->tv_sec * 1000000 + t->tv_usec);
}

/*
** Allocates and return 't_philo_eat_info*' initialized with its arguments.
*/
t_philo_eat_info	*philo_new_philo_info(int philo_index, long int timestamp)
{
	t_philo_eat_info	*ret;

	ret = malloc(sizeof(*ret));
	if (ret == NULL)
		return (NULL);
	ret->philosopher_index = philo_index;
	ret->last_meal_timestamp = timestamp;
	return (ret);
}

/*
** Adds 't1' and 't2' and returns it in microseconds.
*/
long int	philo_add_timeval(struct timeval *t1, struct timeval *t2)
{
	return ((t1->tv_sec + t2->tv_sec) * 1000000 + t1->tv_usec + t2->tv_usec);
}

/*
** First call it with argument to initialize the function's static variables.
** After the first call the function will return the initialized 'mystruct'.
*/
t_philosophers	*philo_get_mystruct(t_philosophers *mystruct)
{
	static t_philosophers	*ptr;
	static bool				is_first_call = true;

	if (is_first_call == true)
	{
		ptr = mystruct;
		is_first_call = false;
	}
	return (ptr);
}

/*
** This function is called before eating.
** Updates last meal timestamp for 'pinfo'.
** Updates the queue of timestamps.
*/
void	philo_enqueue(t_philosopher_info *pinfo)
{
	struct timeval	cur_time;

	gettimeofday(&cur_time, NULL);
	pinfo->last_meal_timestamp = philo_timval_to_microseconds(&cur_time);
	pthread_mutex_lock(pinfo->reference_to_meal_timestamps_mutex);
	ft_fifonodbinenqueue(pinfo->meal_timestamps,
		ft_nodbinnew(philo_new_philo_info(pinfo->philosopher_number - 1,
		pinfo->last_meal_timestamp)));
	// printf("After enqueue:\n");
	// ft_nodbinprint(*pinfo->meal_timestamps);
	pthread_mutex_unlock(pinfo->reference_to_meal_timestamps_mutex);
}
