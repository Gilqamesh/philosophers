/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   race_condition.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 20:21:56 by edavid            #+#    #+#             */
/*   Updated: 2021/09/23 20:37:39 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>

int	mails = 0;
// int lock = 0;
pthread_mutex_t	mutex;

void	*routine()
{
	for (int i = 0; i < 1000000; i++)
	{
		// Idea of mutex in order to fix the race condition:
		// if (lock == 1)
		// 	// wait until the lock is 0
		// lock = 1;
		pthread_mutex_lock(&mutex);
		mails++;
		// lock = 0;
		pthread_mutex_unlock(&mutex);
	}
}

int	main(void)
{
	pthread_t	p1, p2;

	pthread_mutex_init(&mutex, NULL);
	if (pthread_create(&p1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&p2, NULL, &routine, NULL))
		return (2);
	if (pthread_join(p1, NULL))
		return (3);
	if (pthread_join(p2, NULL))
		return (4);
	printf("Number of mails: %d\n", mails);
	pthread_mutex_destroy(&mutex);
	return (0);
}
