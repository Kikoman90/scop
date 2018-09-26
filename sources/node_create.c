/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/25 17:11:15 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_gameobject	*create_gameobject(char *name, unsigned int mtl_id, \
										size_t vc, size_t ic)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
// go->transform = identityMatrix();
	go->vtx_count = vc;
	go->idx_count = ic;
	if (!(go->vertices = (t_vec3*)malloc(sizeof(t_vec3) * vc)) || \
	!(go->indices = (unsigned int*)malloc(sizeof(unsigned int) * ic)))
		return (log_error_null(MALLOC_ERROR));
	go->mtl_id = mtl_id;
	go->name = name;
	return (go);
}

static t_material	*create_material(char *name)
{
	t_material	*mtl;

	if (!(mtl = (t_material*)malloc(sizeof(t_material))))
		return (log_error_null(MALLOC_ERROR));
	mtl->name = name;
	return (mtl);
}

t_go_node			*create_go_node(char *name, unsigned int mtl_id, \
									size_t vc, size_t ic)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	if (!(node->go = create_gameobject(name, mtl_id, vc, ic)))
	{
		clean_go_node(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_mtl_node			*create_mtl_node(char *name)
{
	t_mtl_node	*node;

	if (!(node = (t_mtl_node*)malloc(sizeof(t_mtl_node))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	if (!(node->mtl = create_material(name)))
	{
		clean_mtl_node(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}
