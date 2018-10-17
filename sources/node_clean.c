/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 16:48:51 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/12 17:30:36 by fsidler          ###   ########.fr       */
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

void		clean_go_node(t_go_node *node)
{
	clean_go(node->go);
	node->next = NULL;
	free(node);
	node = NULL;
}

void		clean_mtl_node(t_mtl_node *node)
{
	clean_mtl(node->mtl);
	node->next = NULL;
	free(node);
	node = NULL;
}