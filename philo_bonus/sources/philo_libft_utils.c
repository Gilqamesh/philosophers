/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_libft_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:58:24 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 12:04:22 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

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
