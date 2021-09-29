#include "../philo_bonus/headers/ft_philosophers.h"
#define SEM_NAME "/forks"

int main(void)
{
	sem_t	*forks;
	pid_t	pid;
	int		i;

	sem_unlink(SEM_NAME);
	forks = sem_open(SEM_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0777, 2);
	i = -1;
	while (++i < 5)
	{
		pid = fork();
		if (pid == 0)
		{
			sem_t	*fork;
			fork = sem_open(SEM_NAME, O_RDONLY);
			printf("Waiting from semaphore in child...\n");
			sem_wait(fork);
			printf("Got the semaphore in child\n");
			sem_close(fork);
			exit(EXIT_SUCCESS);
		}
	}
	for (int i = 0; i < 5; i++)
	{
		printf("Waiting for next child process from the parent...\n");
		waitpid(-1, NULL, 0);
		printf("Waited a child process from the parent\n");
	}
	sem_close(forks);
	sem_unlink(SEM_NAME);
}
