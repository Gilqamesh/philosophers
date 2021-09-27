/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 11:51:43 by edavid            #+#    #+#             */
/*   Updated: 2021/09/27 17:26:25 by edavid           ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_philosophers	mystruct;
	t_threads		threads;

	if (argc != 5 && argc != 6)
		return (1);
	if (philo_check_arg_syntax(argc, argv))
		return (1);
	if (philo_init_mystruct(&mystruct, argc, argv))
		return (1);
	if (philo_init_threads(&mystruct, &threads))
		return (1);
	if (philo_join_threads(&mystruct, &threads))
		return (1);
	philo_destroy_mystruct(&mystruct);
	philo_my_free((void **)&threads.philosopher_threads);
	return (0);
}
