/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 20:13:17 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void		redefine_vertices(t_gameobject *go)
{
	unsigned int	i;
	t_vec3			center;

	i = 0;
	center.x = (go->bounds[0] + go->bounds[1]) / 2;
	center.y = (go->bounds[2] + go->bounds[3]) / 2;
	center.z = (go->bounds[4] + go->bounds[5]) / 2;
	while (i < go->vtx_count)
	{
		go->vtx_attrib[i].position = \
			vec3_sub(go->vtx_attrib[i].position, center);
		i++;
	}
}

static void		init_vao_vbo(t_gameobject *go, size_t vtx_struct_size, \
	size_t type_size)
{
	glGenVertexArrays(1, &go->vao);
	glBindVertexArray(go->vao);
	glGenBuffers(1, &go->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, go->vbo);
	glBufferData(GL_ARRAY_BUFFER, vtx_struct_size * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtx_struct_size, \
		(void*)(2 * type_size));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vtx_struct_size, \
		(void*)(5 * type_size));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vtx_struct_size, 0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vtx_struct_size, \
		(void*)(8 * type_size));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

t_vec3			generate_pick_clr(unsigned int id)
{
	float	r;
	float	g;
	float	b;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
	return (vec3_xyz(r / 255.0f, g / 255.0f, b / 255.0f));
}

void			add_mtl_node(t_mtl_list *list, t_mtl_node *node)
{
	t_mtl_node	*tmp;

	if (node)
	{
		list->count++;
		if (!list->head)
		{
			node->id = MTL_ID_OFFSET;
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

	redefine_vertices(node->go);
	init_vao_vbo(node->go, sizeof(t_vtx_attrib), sizeof(float));
	if (node)
	{
		list->count++;
		if (!list->head)
		{
			node->id = GO_ID_OFFSET;
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
