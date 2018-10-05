/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/05 17:42:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void				gl_stack_feed(t_gameobject *go)
{
	glGenVertexArrays(1, &go->gl_stack->vao);
	glBindVertexArray(go->gl_stack->vao);

	glGenBuffers(1, &go->gl_stack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, go->vertices, GL_STATIC_DRAW);
	// (attribute location, size in bytes, type, normalized?, stride, offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &go->gl_stack->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count, go->indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

t_transform			init_transform(void)
{
	t_transform	tr;

	tr.position = vec3_f(0.0f);
	tr.rotation = quat();
	tr.scale = vec3_f(1.0f);
	return (tr);
}

static t_gameobject	*create_gameobject(char *name, unsigned int mtl_id, \
										size_t vc, size_t ic)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
	go->transform = init_transform();
	go->vtx_count = vc;
	go->idx_count = ic;
	if (!(go->vertices = (t_vec3*)malloc(sizeof(t_vec3) * vc)) || \
	!(go->indices = (unsigned int*)malloc(sizeof(unsigned int) * ic)) || \
	!(go->gl_stack = (t_gl_stack*)malloc(sizeof(t_gl_stack))))
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
