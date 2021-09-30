/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_mystruct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 12:14:27 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 20:27:37 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Initializes the arra of philosophers in 'mystruct'.
** All the values are set to default.
*/
static void	init_philosophers_array(t_philosophers *mystruct)
{
	int					i;
	t_philosopher_info	*cur_philo;

	i = -1;
	while (++i < mystruct->phNum)
	{
		cur_philo = &mystruct->ph_arr[i];
		cur_philo->phNum = i + 1;
		cur_philo->time_to_die = mystruct->time_to_die * 1000;
		cur_philo->time_to_eat = mystruct->time_to_eat * 1000;
		cur_philo->time_to_sleep = mystruct->time_to_sleep * 1000;
		if (mystruct->phNum % 2 == 0)
			cur_philo->time_to_think = 0;
		else
		{
			if (cur_philo->time_to_eat - cur_philo->time_to_sleep + 1000 < 0)
				cur_philo->time_to_think = 0;
			else
				cur_philo->time_to_think
					= cur_philo->time_to_eat - cur_philo->time_to_sleep + 1000;
		}
		cur_philo->nOfMeals = mystruct->nOfMeals;
		cur_philo->startTime = mystruct->startTime;
		cur_philo->ateTimestamp = mystruct->startTime;
		cur_philo->semQueue = mystruct->semQueue[i];
		cur_philo->semQueueName = mystruct->semQueueNames[i];
		cur_philo->semDoneEating = mystruct->semDoneEating[i];
		cur_philo->semDoneEatingName = mystruct->semDoneEatingNames[i];
	}
}

int	philo_init_processes(t_philosophers *mystruct)
{
	int	i;

	mystruct->startTime = philo_get_current_timestamp()
		+ INIT_TIME_IN_MICROSECONDS;
	init_philosophers_array(mystruct);
	i = -1;
	while (++i < mystruct->phNum)
	{
		mystruct->process_ids[i] = fork();
		if (mystruct->process_ids[i] == 0)
			philo_process(mystruct, &mystruct->ph_arr[i]);
	}
	return (0);
}

int	philo_kill_processes(t_philosophers *mystruct)
{
	int	i;

	mystruct->allFinishedEating = true;
	i = -1;
	while (++i < mystruct->phNum)
		sem_wait(mystruct->semFinishedEating);
	if (mystruct->allFinishedEating == false)
		pthread_join(mystruct->endCondThread, NULL);
	i = -1;
	while (++i < mystruct->phNum)
	{
		if (mystruct->allFinishedEating == true)
		{
			sem_post(mystruct->semQueue[i]);
			waitpid(mystruct->process_ids[i], NULL, 1);
		}
		else
			kill(mystruct->process_ids[i], SIGTERM);
	}
	return (0);
}


/*
** Initializes 'mystruct' from the program arguments.
** Returns 0 on success, 1 on failure.
** By convention nOfMeals is set to -1 if the
** corresponding argument is not provided.
** All the values are initialized to 0 if not set explicitly.
*/
int	philo_init_mystruct(t_philosophers *mystruct, int argc, char **argv)
{
	int	i;

	memset(mystruct, 0, sizeof(*mystruct));
	philo_get_mystruct(mystruct);
	mystruct->phNum = philo_atoi(argv[1]);
	mystruct->forks = philo_sem_init(SEM_FORKS, mystruct->phNum);
	mystruct->semFinish = philo_sem_init(SEM_FINISH, 0);
	philo_try_malloc((void **)&mystruct->semQueueNames,
		mystruct->phNum * sizeof(*mystruct->semQueueNames));
	philo_try_malloc((void **)&mystruct->semQueue, 
		mystruct->phNum * sizeof(*mystruct->semQueue));
	philo_try_malloc((void **)&mystruct->semDoneEating,
		mystruct->phNum * sizeof(*mystruct->semDoneEating));
	philo_try_malloc((void **)&mystruct->semDoneEatingNames, 
		mystruct->phNum * sizeof(*mystruct->semDoneEatingNames));
	i = -1;
	while (++i < mystruct->phNum)
	{
		mystruct->semQueueNames[i] = ft_strjoin_free(
			ft_strdup(SEM_QUEUE), ft_itoa(i + 1));
		mystruct->semQueue[i] = philo_sem_init(mystruct->semQueueNames[i], 1);
		mystruct->semDoneEatingNames[i] = ft_strjoin_free(
			ft_strdup(SEM_DONE_EATING), ft_itoa(i + 1));
		mystruct->semDoneEating[i] = philo_sem_init(
			mystruct->semDoneEatingNames[i], 1);
	}
	mystruct->semPrint = philo_sem_init(SEM_PRINT, 1);
	mystruct->semFinishedEating = philo_sem_init(SEM_FINISHED_EATING, 0);
	mystruct->time_to_die = philo_atoi(argv[2]);
	mystruct->time_to_eat = philo_atoi(argv[3]);
	mystruct->time_to_sleep = philo_atoi(argv[4]);
	mystruct->nOfMeals = CANT_STOP_EATING;
	if (argc == 6)
		mystruct->nOfMeals = philo_atoi(argv[5]);
	philo_try_malloc((void **)&mystruct->process_ids, mystruct->phNum);
	philo_try_calloc((void **)&mystruct->ph_arr, mystruct->phNum,
		sizeof(*mystruct->ph_arr));
	return (0);
}

/*
** Frees all memory associated with 'mystruct'.
** Returns 1.
*/
int	philo_destroy_mystruct(t_philosophers *mystruct)
{
	int	i;

	philo_my_free((void **)&mystruct->process_ids);
	philo_my_free((void **)&mystruct->ph_arr);
	if (mystruct->allFinishedEating == true)
	{
		sem_post(mystruct->semFinish);
		pthread_join(mystruct->endCondThread, NULL);
	}
	sem_close(mystruct->semFinish); 
	sem_close(mystruct->semFinishedEating);
	sem_close(mystruct->semPrint);
	i = -1;
	while (++i < mystruct->phNum)
	{
		sem_close(mystruct->semQueue[i]);
		sem_unlink(mystruct->semQueueNames[i]);
		philo_my_free((void **)&mystruct->semQueueNames[i]);
	}
	philo_my_free((void **)&mystruct->semQueue);
	philo_my_free((void **)&mystruct->semQueueNames);
	philo_my_free((void **)&mystruct->semDoneEating);
	philo_my_free((void **)&mystruct->semDoneEatingNames);
	sem_close(mystruct->forks);
	sem_unlink(SEM_FINISH);
	sem_unlink(SEM_FINISHED_EATING);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FORKS);
	memset(mystruct, 0, sizeof(*mystruct));
	return (1);
}
