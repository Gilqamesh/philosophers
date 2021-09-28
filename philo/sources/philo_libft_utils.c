/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_libft_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:58:24 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 17:03:45 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			break ;
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
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
		if (result > INT_MAX / 10 || (result == INT_MAX / 10 && *str > '7'))
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
** Returns max out of 'a' and 'b'.
*/
long int	long_int_max_of(long int a, long int b)
{
	if (a > b)
		return (a);
	return (b);
}
