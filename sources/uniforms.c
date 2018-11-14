/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 19:11:34 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 19:43:02 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	set_def_uniforms(t_shader shader, float fade, GLuint tex_id)
{
	
}

static void	set_std_uniforms(t_shader shader, t_material *mtl)
{
	glUniform3fv(shader.u_loc[7], 1, &mtl->clr_amb.x);
	glUniform3fv(shader.u_loc[8], 1, &mtl->clr_dif.x);
	glUniform3fv(shader.u_loc[9], 1, &mtl->clr_spc.x);
	glUniform1f(shader.u_loc[10], mtl->expnt_spc);
	glUniform1f(shader.u_loc[11], mtl->transparency);
}

void		set_uniforms(t_env *env, t_go_node *node, unsigned int idx)
{
	t_shader	shader;
	t_vec3		view;

	shader = env->shaders[idx];
	glUniformMatrix4fv(shader.u_loc[0], 1, GL_FALSE, \
		&(env->matrices.model[node->id - 1].m[0]));
	glUniformMatrix4fv(shader.u_loc[1], 1, GL_FALSE, &(env->matrices.vp.m[0]));
	if (idx == 1)
		glUniform3fv(shader.u_loc[2], 1, &node->go->pick_clr.x);
	else
	{
		//view = vector_rot(env->camera.transform.rotation, (t_vec3)VEC3_FRONT);
		view = env->camera.transform.position;
		glUniform3fv(shader.u_loc[2], 1, &view.x);
		glUniform3fv(shader.u_loc[3], 1, &env->light.transform.position.x);
		glUniform3fv(shader.u_loc[4], 1, &env->light.color.x);
		glUniform1f(shader.u_loc[5], env->light.intensity);
		glUniform1f(shader.u_loc[6], env->light.range);
		if (idx == 0)
			set_def_uniforms(shader, env->fade, env->tex.id);
		else if (idx == 2)
			set_std_uniforms(shader, \
				get_mtl(env->materials.head, node->go->mtl_id));
	}
}


