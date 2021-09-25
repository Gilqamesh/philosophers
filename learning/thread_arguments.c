#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int	primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void	*routine(void *arg)
{
	printf("%d ", *(int *)arg);
	return (arg);
}

int	main(void)
{
	pthread_t	th[10];

	for (int i = 0; i < 10; i++)
	{
		// int	*a = malloc(sizeof(int));
		// *a = i;
		if (pthread_create(th + i, NULL, &routine, primes + i))
			return (1);
	}
	for (int i = 0; i < 10; i++)
	{
		// int	*a;
		if (pthread_join(th[i], NULL))
			return (2);
		// free(a);
	}
	return (0);
}
