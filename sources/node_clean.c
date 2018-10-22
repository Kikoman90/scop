/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 16:48:51 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/22 16:24:34 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	clean_go(t_gameobject *go)
{
	if (go->vertices)
	{
		free(go->vertices);
		go->vertices = NULL;
	}
	if (go->indices)
	{
		free(go->indices);
		go->indices = NULL;
	}
	if (go->gl_stack)
	{
		glDeleteBuffers(1, &go->gl_stack->vbo);
		glDeleteBuffers(1, &go->gl_stack->ibo);
		glDeleteVertexArrays(1, &go->gl_stack->vao);
		free(go->gl_stack);
		go->gl_stack = NULL;
	}
	free(go->name);
	free(go);
	go = NULL;
}

static void	clean_mtl(t_material *mtl)
{
	free(mtl->name);
	free(mtl);
	mtl = NULL;
}

void		clean_go_node(t_go_node *node, unsigned int no_free)
{
	if (node->go && !no_free)
		clean_go(node->go);
	node->next = NULL;
	free(node);
	node = NULL;
}

void		clean_mtl_node(t_mtl_node *node, unsigned int no_free)
{
	if (node->mtl && !no_free)
		clean_mtl(node->mtl);
	node->next = NULL;
	free(node);
	node = NULL;
}

void		remove_go_node(t_go_node *list, unsigned int id, \
	unsigned int no_f, size_t *count)
{
	t_go_node		*tmp;
	t_go_node		*tmp_prev;

	tmp = list;
	tmp_prev = tmp;
	while (tmp)
	{
		if (id == tmp->id)
		{
			if (tmp_prev == tmp)
				list = tmp_prev->next;
			else
				tmp_prev->next = tmp->next;
			clean_go_node(tmp, no_f);
			id = 0;
			*count -= 1;
			tmp = tmp_prev->next;
		}
		else
		{
			tmp->id -= (id == 0) ? 1 : 0;
			tmp_prev = tmp;
			tmp = tmp->next;
		}
	}
}