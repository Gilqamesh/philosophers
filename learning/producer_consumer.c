/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   producer_consumer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 20:06:30 by edavid            #+#    #+#             */
/*   Updated: 2021/09/25 11:27:34 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Unfortunately sem_init and sem_destroy does not seem to be implemented.
// (__deprecated)

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define N_OF_THREADS 8
#define BUFFER_SIZE 10

int	buffer[BUFFER_SIZE];
int	cur_size = 0;

sem_t			semEmpty;
sem_t			semFull;
pthread_mutex_t	mutexBuffer;

void	*producer()
{
	while (1)
	{
		// produce
		int	x = rand() % 100;
		sleep(1);
		sem_wait(&semEmpty);
		pthread_mutex_lock(&mutexBuffer);
		buffer[cur_size++] = x;
		pthread_mutex_unlock(&mutexBuffer);
		sem_post(&semFull);
	}
}

void	*consumer()
{
	while (1)
	{
		int	y;
		sem_wait(&semFull);
		pthread_mutex_lock(&mutexBuffer);
		y = buffer[--cur_size];
		pthread_mutex_unlock(&mutexBuffer);
		sem_post(&semEmpty);
		// consume
		printf("Got %d\n", y);
	}
}

int	main(void)
{
	pthread_t	th[N_OF_THREADS];

	srand(time(NULL));
	sem_init(&semEmpty, 0, BUFFER_SIZE);
	sem_init(&semFull, 0, 0);
	pthread_mutex_init(&mutexBuffer, NULL);
	for (int i = 0; i < N_OF_THREADS; i++)
	{
		if (i % 2 && pthread_create(th + i, NULL, &producer, NULL))
			return (1);
		else if (pthread_create(th + i, NULL, &consumer, NULL))
			return (2);
	}
	for (int i = 0; i < N_OF_THREADS; i++)
		if (pthread_join(th[i], NULL))
			return (3);
	sem_destroy(&semEmpty);
	sem_destroy(&semFull);
	pthread_mutex_destroy(&mutexBuffer);
	return (0);
}
