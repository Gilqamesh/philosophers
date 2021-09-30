/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_libft_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:58:24 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 15:03:12 by edavid           ###   ########.fr       */
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

int	ft_strlen(const char *s)
{
	int	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (*s)
	{
		len++;
		s++;
	}
	return (len);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*new;

	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	new = (char *)malloc(len + 1);
	if (!new)
		return ((char *)0);
	ft_strlcpy(new, s, len + 1);
	return (new);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;

	if (!src)
		return (0);
	len = ft_strlen(src);
	if (size)
	{
		while (--size && *src)
			*dst++ = *src++; 
		*dst = '\0';
	}
	return (len);
}

static char	*strjoin_return_helper(char *s1, char *s2)
{
	if (s1 == NULL && s2 != NULL)
		return (s2);
	if (s1 != NULL && s2 == NULL)
		return (s1);
	return (NULL);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*new;
	char	*r;
	char	*tmp;

	if (s1 == NULL || s2 == NULL)
		return (strjoin_return_helper(s1, s2));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = (char *)malloc(s1_len + s2_len + 1);
	if (!new)
		return (NULL);
	r = new;
	tmp = s1;
	while (s1_len--)
		*r++ = *tmp++;
	free(s1);
	tmp = s2;
	while (s2_len--)
		*r++ = *tmp++;
	free(s2);
	*r = '\0';
	return (new);
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
