#include "../philo/headers/ft_philosophers.h"
#define N_OF_THREADS 6

int				fuel = 0;
pthread_mutex_t	mutexFuel;
pthread_cond_t	condFuel;
// pthread_mutex_t	mutexCondBool;
// bool			isLocked;
// pthread_mutex_t	mutexCond;

void	*fuel_filling(void *arg)
{
	(void)arg;
	for (int i = 0; i < 5; i++)
	{
		pthread_mutex_lock(&mutexFuel);
		fuel += 60;
		printf("Filled fuel... %d\n", fuel);

		// pthread_mutex_lock(&mutexCondBool);
		// if (isLocked == true)
		// {
		// 	isLocked = false;
		// 	pthread_mutex_unlock(&mutexCond);
		// }
		// pthread_mutex_unlock(&mutexCondBool);

		pthread_mutex_unlock(&mutexFuel);
		// Not guaranteed that it signals multiple threads
		// pthread_cond_signal(&condFuel);
		// Broadcast for all threads with the specific signal
		pthread_cond_broadcast(&condFuel);
		sleep(1);
	}
	return (NULL);
}

void	*car(void *arg)
{
	// bool	unlockedFuel;
	(void)arg;
	pthread_mutex_lock(&mutexFuel);
	// unlockedFuel = false;
	while (fuel < 40)
	{
		printf("No fuel. Waiting...\n");

		// pthread_mutex_lock(&mutexCondBool);
		// if (isLocked == false)
		// 	isLocked = true;
		// pthread_mutex_unlock(&mutexCondBool);
		// pthread_mutex_unlock(&mutexFuel);
		// unlockedFuel = true;
		// pthread_mutex_lock(&mutexCond);

		pthread_cond_wait(&condFuel, &mutexFuel);
		// Equivalent to:
		// pthread_mutex_unlock(&mutexFuel);
		// wait for signal on condFuel
		// pthread_mutex_lock(&mutexFuel);
	}
	fuel -= 40;
	printf("Got fuel. Now left: %d\n", fuel);
	// if (unlockedFuel == false)
	pthread_mutex_unlock(&mutexFuel);
	return (NULL);
}

int	main(void)
{
	pthread_t	th[N_OF_THREADS];
	pthread_mutex_init(&mutexFuel, NULL);
	pthread_cond_init(&condFuel, NULL);

	// isLocked = true;
	// pthread_mutex_init(&mutexCondBool, NULL);
	// pthread_mutex_init(&mutexCond, NULL);
	// pthread_mutex_lock(&mutexCond);

	for (int i = 0; i < N_OF_THREADS; i++)
	{
		if (i == 4 || i == 5)
		{
			if (pthread_create(&th[i], NULL, &fuel_filling, NULL))
				return (1);
		}
		else if (pthread_create(&th[i], NULL, &car, NULL))
			return (2);
	}
	for (int i = 0; i < N_OF_THREADS; i++)
		if (pthread_join(th[i], NULL))
			return (3);

	// pthread_mutex_destroy(&mutexCondBool);
	// pthread_mutex_destroy(&mutexCond);

	pthread_mutex_destroy(&mutexFuel);
	pthread_cond_destroy(&condFuel);
	return (0);
}
