/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_queue_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 12:31:54 by edavid            #+#    #+#             */
/*   Updated: 2021/09/26 19:48:25 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

/*
** Add an element to the end of the queue.
*/
void	ft_fifonodbinenqueue(t_node_binary **lst, t_node_binary *new)
{
	if (lst == NULL || new == NULL)
		return ;
	ft_nodbinadd_front(lst, new);
}

/*
** Remove and return an element from the front of the queue.
** 'nullable_last_node' refers to the last node in 'lst', in which case the
** function works much faster as it doesn't have to travel through the 'lst'.
*/
t_node_binary	*ft_fifonodbindequeue(t_node_binary **lst,
t_node_binary *nullable_last_node)
{
	t_node_binary	*cur;

	if (lst == NULL || *lst == NULL)
		return (NULL);
	if (nullable_last_node)
	{
		cur = nullable_last_node;
		if (cur->prev)
			cur->prev->next = NULL;
		else
			*lst = NULL;
		return (cur);
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	if (cur->prev)
		cur->prev->next = NULL;
	else
		*lst = NULL;
	return (cur);
}

/*
** Returns true if the queue is empty, false otherwise.
*/
bool	ft_fifolst_is_empty(t_node_binary *lst)
{
	if (lst == NULL)
		return (false);
	return (true);
}
