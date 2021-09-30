/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:39:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 20:24:41 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Checks if each 'argv' is a valid number or not.
** Returns 0 on success, 1 on failure.
*/
static int	philo_check_arg_syntax(int argc, char **argv)
{
	while (--argc)
		if (philo_is_valid_positive_number(argv[argc]) == false)
			return (1);
	return (0);
}

void	*endCond(void *mystructPtr)
{
	t_philosophers	*mystruct;
	int				i;

	mystruct = (t_philosophers *)mystructPtr;
	sem_wait(mystruct->semFinish);
	mystruct->allFinishedEating = false;
	i = -1;
	while (++i < mystruct->phNum)
		sem_post(mystruct->semFinishedEating);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philosophers	mystruct;

	if (argc != 5 && argc != 6)
		return (1);
	if (philo_check_arg_syntax(argc, argv))
		return (1);
	if (philo_init_mystruct(&mystruct, argc, argv))
		return (1);
	pthread_create(&mystruct.endCondThread, NULL, &endCond, &mystruct);
	if (philo_init_processes(&mystruct))
		return (1);
	if (philo_kill_processes(&mystruct))
		return (1);
	philo_destroy_mystruct(&mystruct);
	return (0);
}
