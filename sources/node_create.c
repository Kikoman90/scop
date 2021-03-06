/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:32:00 by fsidler          ###   ########.fr       */
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
	size_t vc)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
	go->name = name;
	go->transform = init_transform_trs(\
		vec3_xyz(0, 0, -3), quat_tv(0, (t_vec3)VEC3_UP), (t_vec3)VEC3_ONE);
	if (!(go->vtx_attrib = (t_vtx_attrib*)malloc(sizeof(t_vtx_attrib) * vc)))
		return (log_error_null(MALLOC_ERROR));
	go->vtx_count = vc;
	go->vao = 0;
	go->vbo = 0;
	go->mtl_id = mtl_id;
	go->pick_clr = (t_vec3)VEC3_ZERO;
	go->bounds[0] = 2000.0f;
	go->bounds[1] = -2000.0f;
	go->bounds[2] = 2000.0f;
	go->bounds[3] = -2000.0f;
	go->bounds[4] = 2000.0f;
	go->bounds[5] = -2000.0f;
	return (go);
}

t_go_node			*create_go_node(char *name, unsigned int mtl_id, size_t vc)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	if (!(node->go = create_gameobject(name, mtl_id, vc)))
	{
		clean_go_node(node, 1);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_tr_node			*create_tr_node(unsigned int id, t_transform *transform)
{
	t_tr_node	*node;

	if (!(node = (t_tr_node*)malloc(sizeof(t_tr_node))))
		return (log_error_null(MALLOC_ERROR));
	node->id = id;
	node->transform = transform;
	node->next = NULL;
	return (node);
}
