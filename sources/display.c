/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:23:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 11:23:39 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <stdio.h>

// remove this file

void		display_gameobject(t_gameobject *obj)
{
	unsigned int i;

	i = 0;
	printf("display object (name : %s)\n", obj->name);
	printf("material id : %d\n", obj->mtl_id);
	while (i < obj->vtx_count)
	{
		printf("vertex %d = (%f, %f, %f)\n", i, obj->vertices[i].x, obj->vertices[i].y, obj->vertices[i].z);
		i++;
	}
	i = 0;
	while (i < obj->idx_count)
	{
		printf("triangle %d = (%d, %d, %d)\n", i / 3, obj->indices[i], obj->indices[i + 1], obj->indices[i + 2]);
		i += 3;
	}
}

void		display_go_list(t_go_node *list)
{
	t_go_node	*tmp;

	tmp = list;
	printf("display obj list\n");
	while (tmp)
	{
		display_gameobject(tmp->go);
		tmp = tmp->next;
	}
}
