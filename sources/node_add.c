/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 22:04:29 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void			add_mtl_node(t_mtl_list *list, t_mtl_node *node)
{
	t_mtl_node	*tmp;

	if (node)
	{
		node->id = ++list->count;
		if (!list->head)
			list->head = node;
		else
		{
			tmp = list->head;
			while (tmp && tmp->next)
				tmp = tmp->next;
			tmp->next = node;
		}
	}
}

static t_vec3	generate_pick_clr(unsigned int id)
{
	float	r;
	float	g;
	float	b;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
	return (vec3_xyz(r / 255.0f, g / 255.0f, b / 255.0f));
}

void			add_go_node(t_go_list *list, t_go_node *node)
{
	t_go_node	*tmp;

	if (node)
	{
		node->id = ++list->count;
		node->go->pick_clr = generate_pick_clr(node->id);
		if (!list->head)
			list->head = node;
		else
		{
			tmp = list->head;
			while (tmp && tmp->next)
				tmp = tmp->next;
			tmp->next = node;
		}
	}
}
