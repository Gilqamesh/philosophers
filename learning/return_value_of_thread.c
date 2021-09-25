/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_value_of_thread.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 13:40:18 by edavid            #+#    #+#             */
/*   Updated: 2021/09/24 14:35:26 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void	*roll_dice()
{
	int *value;

	value = malloc(sizeof(int));
	*value = (rand() % 6) + 1;
	return ((void *)value);
}

int	main(void)
{
	pthread_t	th;
	int			*res;

	srand(time(NULL));
	if (pthread_create(&th, NULL, &roll_dice, NULL))
		return (1);
	if (pthread_join(th, (void **)&res))
		return (2);
	printf("Result: %d\n", *res);
	free(res);
	return (0);
}
