/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:34:29 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_material	*create_material(char *name)
{
	t_material	*mtl;

	if (!(mtl = (t_material*)malloc(sizeof(t_material))))
		return (log_error_null(MALLOC_ERROR));
	mtl->name = name;
	mtl->clr_amb = (t_vec3)VEC3_ZERO;
	mtl->clr_dif = (t_vec3)VEC3_ZERO;
	mtl->clr_spc = (t_vec3)VEC3_ZERO;
	mtl->expnt_spc = 0.0f;
	mtl->transparency = 1.0f;
	return (mtl);
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
		clean_mtl_node(node, 0);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_gameobject	*create_gameobject(char *name, unsigned int mtl_id, \
	size_t ic)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
	go->transform = init_transform_trs(\
		vec3_xyz(0, 0, -3), quat_tv(90, (t_vec3)VEC3_UP), (t_vec3)VEC3_ONE);
	go->idx_count = ic;
	if (!(go->indices = (unsigned int*)malloc(sizeof(unsigned int) * ic)) || \
	!(go->gl_stack = (t_gl_stack*)malloc(sizeof(t_gl_stack))))
		return (log_error_null(MALLOC_ERROR));
	go->mtl_id = mtl_id;
	go->name = name;
	return (go);
}

t_go_node			*create_go_node(char *name, unsigned int mtl_id, size_t ic)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	if (!(node->go = create_gameobject(name, mtl_id, ic)))
	{
		clean_go_node(node, 0);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}