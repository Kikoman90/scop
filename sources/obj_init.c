/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:15:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 21:51:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void				init_gl_objects(t_gameobject *go, char *b, size_t sf)
{
	/*printf("SIZEOF(float) = %zu (%zu)\n", sizeof(float), sf);
	printf("SIZEOF(t_idx_attrib) = %zu\n", sizeof(t_vtx_attrib));
	printf("3 * sizeof(float) vs sizeof(t_vec3) = %zu / %zu\n", 3 * sizeof(float), sizeof(t_vec3));*/

	// glGetAttribLocation ("iPosition")... etc.
	// create init_vbo function -> 1 vbo ? -> glBufferData then glBufferSubData
	// 4 vbos -> glBufferData;

	glGenVertexArrays(1, &go->gl_stack->vao);
	glBindVertexArray(go->gl_stack->vao);
	glGenBuffers(4, &go->gl_stack->vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(2 * sizeof(float)));
	// b + (2 * sf));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vtx_attrib), b + (5 * sf));
	// b + (5 * sf));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t_vtx_attrib), 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec2) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(t_vtx_attrib), b + (8 * sf));
	//	(char *)NULL + (8 * sizeof(float)));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glGenBuffers(1, &go->gl_stack->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count,\
		&go->indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0); // remove me
	/*GLfloat	dub[4];
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[0]);
	glGetVertexAttribfv(3, GL_CURRENT_VERTEX_ATTRIB, dub);
	printf("DUB = {%f %f %f %f}\n", dub[0], dub[1], dub[2], dub[3]);*/
}

t_mat4x4			go_trs(t_transform tr)
{
	return (mat4x4_trs(tr.position, tr.rotation, tr.scale));
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

t_light				init_light(t_env *env, t_vec3 color, float i, float range)
{
	t_light			light;

	light.go = get_gameobject(env->go_list, env->go_count);
	light.go->transform.position = vec3_xyz(2.3f, 3.0f, 0.0f);
	light.go->transform.scale = vec3_xyz(0.2f, 0.2f, 0.2f);
	light.light_color = color;
	light.intensity = i;
	light.range = range;
	remove_go_node(env->go_list, env->go_count, 1, &env->go_count);
	return (light);
}