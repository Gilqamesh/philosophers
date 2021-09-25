#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define N_OF_THREADS 12

int	water = 0;
int	fuel = 50;

pthread_mutex_t	mutexFuel;
pthread_mutex_t	mutexWater;

void	*routine()
{
	if (rand() % 2 == 0)
	{
		pthread_mutex_lock(&mutexFuel);
		sleep(1);
		pthread_mutex_lock(&mutexWater);
	}
	else
	{
		pthread_mutex_lock(&mutexWater);
		sleep(1);
		pthread_mutex_lock(&mutexFuel);
	}
	fuel += 50;
	water += 25;
	printf("Incremented fuel to: %d and water to: %d\n", fuel, water);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexWater);
}

int	main(void)
{
	pthread_t	th[12];

	pthread_mutex_init(&mutexFuel, NULL);
	pthread_mutex_init(&mutexWater, NULL);
	for (int i = 0; i < 10; i++)
		if (pthread_create(th + i, NULL, &routine, NULL))
			return (1);
	for (int i = 0; i < 10; i++)
		if (pthread_join(th[i], NULL))
			return (2);
	pthread_mutex_destroy(&mutexFuel);
	pthread_mutex_destroy(&mutexWater);
	return (0);
}
