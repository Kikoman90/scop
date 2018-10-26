/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:15:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/26 15:33:09 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void				init_gl_objects(t_gameobject *go)
{
	glGenVertexArrays(1, &go->gl_stack->vao);
	glBindVertexArray(go->gl_stack->vao);
	glGenBuffers(4, &go->gl_stack->vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * GL_FLOAT, \
		(char *)NULL + (2 * sizeof(float)));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * GL_FLOAT, \
		(char *)NULL + (5 * sizeof(float)));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * GL_FLOAT, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec2) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * GL_FLOAT, \
		(char *)NULL + (8 * sizeof(float)));
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3) * go->vtx_count, \
		&go->vtx_attrib[0], GL_STATIC_DRAW);
	glGenBuffers(1, &go->gl_stack->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count,\
		&go->indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
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
	t_gameobject	*light_go;

	light_go = get_go_node(env->go_list, env->go_count)->go;
	light.go = light_go;
	light.go->transform.position = vec3_xyz(2.3f, 3.0f, 0.0f);
	light.go->transform.scale = vec3_xyz(0.2f, 0.2f, 0.2f);
	light.light_color = color;
	light.intensity = i;
	light.range = range;
	remove_go_node(env->go_list, env->go_count, 1, &env->go_count);
	return (light);
}