/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/16 15:01:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void				generate_pick_clr(t_gameobject *go, unsigned int id)
{
	float	r;
	float	g;
	float	b;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
	go->pick_clr = vec3_xyz(r / 255, g / 255, b / 255);
}

t_go_node			*add_go_node(t_env *env, t_go_node *node)
{
	t_go_node	*head;

	head = env->go_list;
	env->go_count++;
	node->id = env->go_count;
	generate_pick_clr(node->go, node->id);
	if (head == NULL)
		return (node);
	while (env->go_list->next)
		env->go_list = env->go_list->next;
	env->go_list->next = node;
	return (head);
}

t_mtl_node			*add_mtl_node(t_env *env, t_mtl_node *node)
{
	t_mtl_node	*head;

	head = env->mtl_list;
	env->mtl_count++;
	node->id = env->mtl_count;
	if (head == NULL)
		return (node);
	while (env->mtl_list->next)
		env->mtl_list = env->mtl_list->next;
	env->mtl_list->next = node;
	return (head);
}
