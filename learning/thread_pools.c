#include "../philo/headers/ft_philosophers.h"
#define N_OF_THREADS 10

typedef struct Task
{
	int a;
	int b;
}	Task;

Task			taskQueue[256];
int				taskCount = 0;
pthread_mutex_t	mutexQueue;
pthread_cond_t	condQueue;

void	submitTask(Task *task)
{
	pthread_mutex_lock(&mutexQueue);
	if (taskCount < 256)
	{
		taskQueue[taskCount++] = *task;
		pthread_mutex_unlock(&mutexQueue);
		pthread_cond_signal(&condQueue);
	}
	pthread_mutex_unlock(&mutexQueue);
}

void	executeTask(Task *task)
{
	usleep(100000);
	int	result = task->a + task->b;
	printf("The sum of %d and %d is %d\n", task->a, task->b, result);
}

void	*startThread(void *args)
{
	(void)args;
	while (1)
	{
		Task	task;
		pthread_mutex_lock(&mutexQueue);
		while (taskCount == 0)
			pthread_cond_wait(&condQueue, &mutexQueue);
		task = taskQueue[0];
		for (int i = 0; i < taskCount - 1; i++)
			taskQueue[i] = taskQueue[i + 1];
		taskCount--;
		pthread_mutex_unlock(&mutexQueue);
		executeTask(&task);
	}
}

int	main(void)
{
	pthread_t	th[N_OF_THREADS];

	pthread_mutex_init(&mutexQueue, NULL);
	pthread_cond_init(&condQueue, NULL);
	for (int i = 0; i < N_OF_THREADS; i++)
		if (pthread_create(&th[i], NULL, &startThread, NULL))
			return (1);
	srand(time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		Task t = {
			.a = rand() % 100,
			.b = rand() % 100
		};
		submitTask(&t);
	}
	for (int i = 0; i < 4; i++)
		if (pthread_join(th[i], NULL))
			return (1);
	pthread_mutex_destroy(&mutexQueue);
	pthread_cond_destroy(&condQueue);
	return (0);
}
