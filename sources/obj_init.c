/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:15:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/30 16:26:20 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

#include <stddef.h> //remove ?

void				init_gl_objects(t_gameobject *go, size_t buf_s, \
	size_t attr_s)
{
	glGenVertexArrays(1, &go->gl_stack->vao);
	glBindVertexArray(go->gl_stack->vao);
	glGenBuffers(1, &go->gl_stack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack->vbo);
	glBufferData(GL_ARRAY_BUFFER, buf_s * go->vtx_count, &go->vtx_attrib[0], \
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(2 * attr_s));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(5 * attr_s));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, buf_s, 0);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(8 * attr_s));
	glGenBuffers(1, &go->gl_stack->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count,\
		&go->indices[0], GL_STATIC_DRAW);
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
	light.go->transform.position = vec3_xyz(0.5f, 1.2f, 0.0f);
	light.go->transform.scale = vec3_xyz(0.2f, 0.2f, 0.2f);
	light.light_color = color;
	light.intensity = i;
	light.range = range;
	remove_go_node(env->go_list, env->go_count, 1, &env->go_count);
	return (light);
}