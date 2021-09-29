/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_nodbin_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 12:29:03 by edavid            #+#    #+#             */
/*   Updated: 2021/09/28 17:34:36 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_philosophers.h"

t_node_binary	*ft_nodbinnew(void *content)
{
	t_node_binary	*new_el;

	new_el = malloc(sizeof(*new_el));
	if (new_el == NULL)
		return (NULL);
	new_el->content = content;
	new_el->next = NULL;
	new_el->prev = NULL;
	return (new_el);
}

void	ft_nodbinadd_front(t_node_binary **lst, t_node_binary *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
	if (new->next)
		new->next->prev = new;
	new->prev = NULL;
}

void	ft_nodbinclear(t_node_binary **lst, void (*del)(void *), int n)
{
	t_node_binary	*tmp;
	t_node_binary	*cur;

	if (lst == NULL || del == NULL)
		return ;
	cur = *lst;
	while (cur && n)
	{
		tmp = cur->next;
		ft_nodbindelone(cur, del);
		cur = tmp;
		if (n > 0 && !--n)
			break ;
	}
	*lst = NULL;
}

void	ft_nodbindelone(t_node_binary *lst, void (*del)(void *))
{
	if (lst == NULL || del == NULL)
		return ;
	(*del)(lst);
}

void	ft_nodbindel(void *node)
{
	if (node == NULL)
		return ;
	if (((t_node_binary *)node)->content)
		free(((t_node_binary *)node)->content);
	free(node);
}
