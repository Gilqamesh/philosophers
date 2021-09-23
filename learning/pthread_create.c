/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:07:25 by edavid            #+#    #+#             */
/*   Updated: 2021/09/23 15:55:50 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	*routine()
{
	printf("Test from threads\n");
	sleep(3);
	printf("Ending thread\n");
}

int	main(void)
{
	pthread_t	t1, t2;

	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
	{
		// Possible error returns for pthread_create:
		// EAGAIN: 	Insufficient resources to create another thread.
		// EINVAL: 	Invalid settings in attr.
		// EPERM:  	No permission to set the scheduling policy and parameters
        //			specified in attr.
		return (1);
	}
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
	{
		return (2);
	}
	if (pthread_join(t1, NULL) != 0)
	{
		// Possible error returns for pthread_join:
		// EDEADLK:	A deadlock was detected (e.g., two threads tried to join
		//			with each other); or thread specifies the calling thread.
		//
		return (3);
	}
	if (pthread_join(t2, NULL) != 0)
	{
		return (4);
	}
	return (0);
}
