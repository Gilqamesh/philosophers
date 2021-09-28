/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_malloc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:57:06 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 19:04:06 by edavid           ###   ########.fr       */
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
** Allocates and return 't_philo_eat_info*' initialized with its arguments.
*/
t_philo_eat_info	*philo_new_philo_info(int philo_index, long int timestamp)
{
	t_philo_eat_info	*ret;

	ret = malloc(sizeof(*ret));
	if (ret == NULL)
		return (NULL);
	ret->philosopher_index = philo_index;
	ret->ateTimestamp = timestamp;
	return (ret);
}
