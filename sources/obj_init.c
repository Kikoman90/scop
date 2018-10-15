/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:15:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/15 11:16:51 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void				init_gl_objects(t_gameobject *go)
{
	glGenVertexArrays(1, &go->gl_stack->vao);
	glBindVertexArray(go->gl_stack->vao);
	glGenBuffers(1, &go->gl_stack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, &go->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &go->gl_stack->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count, &go->indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

t_transform			init_transform(void)
{
	t_transform	tr;

	tr.position = vec3_xyz(0, 0, 0);
	tr.rotation = quat();
	tr.scale = vec3_f(1);
	return (tr);
}

t_transform         init_transform_trs(t_vec3 t, t_quaternion r, t_vec3 s)
{
    t_transform	tr;

	tr.position = t;
	tr.rotation = r;
	tr.scale = s;
	return (tr);
}