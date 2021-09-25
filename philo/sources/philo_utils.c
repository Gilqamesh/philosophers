/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 13:23:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/25 21:47:44 by edavid           ###   ########.fr       */
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
** Calculates the difference in milliseconds between 'current' and 'previous'.
*/
int	philo_calc_ms_difference(struct timeval *current, struct timeval *previous)
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
** Updates most hungry philosopher's index and time till starvation.
** This function should be updated each time before the reaper sleeps.
** Returns 1 
*/
// int	philo_update_most_hungry()
// {
// 	return ()
// }
