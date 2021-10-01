/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_libft_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 12:03:28 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 12:04:28 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

int	ft_intlen(int d)
{
	int	len;

	if (!d)
		return (1);
	len = 0;
	while (d)
	{
		d /= 10;
		len++;
	}
	return (len);
}

long int	*ft_lintdup(long int li)
{
	long int	*liPtr;

	liPtr = malloc(sizeof(*liPtr));
	if (liPtr == NULL)
		return (NULL);
	*liPtr = li;
	return (liPtr);
}

static char	*ft_initstr(char **str, int len, int is_negative, int is_zero)
{
	*str = (char *)malloc(len + 1);
	if (!*str)
		return ((char *)0);
	*(*str + len) = '\0';
	if (is_negative)
		**str = '-';
	if (is_zero)
		**str = '0';
	return (*str);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	int		negative;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = ft_intlen(n);
	negative = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
		negative = 1;
	}
	if (!ft_initstr(&str, len, negative, !n))
		return ((char *)0);
	while (n)
	{
		*(str + --len) = n % 10 + '0';
		n /= 10;
	}
	return (str);
}
