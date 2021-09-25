/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_in_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 13:26:49 by edavid            #+#    #+#             */
/*   Updated: 2021/09/24 13:34:19 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#define N_OF_THREADS 12

pthread_mutex_t	mutex;
int				mails = 0;

void	*routine()
{
	static int	calls;

	printf("Current number of calls: %d\n", calls++);
	for (int i = 0; i < 10000000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
}

int	main(void)
{
	pthread_t	th[N_OF_THREADS];

	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < N_OF_THREADS; i++)
	{
		if (pthread_create(th + i, NULL, &routine, NULL))
			return (1);
		printf("Thread %d has started\n", i);
	}
	for (int i = 0; i < N_OF_THREADS; i++)
	{
		if (pthread_join(th[i], NULL))
			return (2);
		printf("Thread %d has finished execution\n", i);
	}
	printf("Number of mails: %d\n", mails);
	pthread_mutex_destroy(&mutex);
	return (0);
}
