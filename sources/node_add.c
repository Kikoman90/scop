/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 19:55:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void			add_mtl_node(t_mtl_list *list, t_mtl_node *node)
{
	t_mtl_node	*tmp;

	if (node)
	{
		list->count++;
		if (!list->head)
		{
			node->id = 1;
			list->head = node;
		}
		else
		{
			tmp = list->head;
			while (tmp && tmp->next)
				tmp = tmp->next;
			node->id = tmp->id + 1;
			tmp->next = node;
		}
	}
}

void			add_go_node(t_go_list *list, t_go_node *node)
{
	t_go_node	*tmp;

	if (node)
	{
		list->count++;
		if (!list->head)
		{
			node->id = 20;
			list->head = node;
		}
		else
		{
			tmp = list->head;
			while (tmp && tmp->next)
				tmp = tmp->next;
			node->id = tmp->id + 1;
			tmp->next = node;
		}
		node->go->pick_clr = generate_pick_clr(node->id);
	}
}
