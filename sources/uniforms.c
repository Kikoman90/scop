/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 19:11:34 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:14:01 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	set_def_uniforms(t_shader shader, t_light light, float fade) // and a texture
{
	glUniform3fv(shader.u_loc[3], 1, &light.go->transform.position.x);
	glUniform3fv(shader.u_loc[4], 1, &light.light_color.x);
	glUniform1f(shader.u_loc[5], light.intensity);
	glUniform1f(shader.u_loc[6], light.range);
	glUniform1f(shader.u_loc[7], fade);
	// [8] texture uniform
}

static void	set_std_uniforms(t_shader shader, t_light light, t_material *mtl)
{
	glUniform3fv(shader.u_loc[3], 1, &light.go->transform.position.x);
	glUniform3fv(shader.u_loc[4], 1, &light.light_color.x);
	glUniform1f(shader.u_loc[5], light.intensity);
	glUniform1f(shader.u_loc[6], light.range);
	glUniform3fv(shader.u_loc[7], 1, &mtl->clr_amb.x);
	glUniform3fv(shader.u_loc[8], 1, &mtl->clr_dif.x);
	glUniform3fv(shader.u_loc[9], 1, &mtl->clr_spc.x);
	glUniform1f(shader.u_loc[10], mtl->expnt_spc);
	glUniform1f(shader.u_loc[11], mtl->transparency);
}

// 27 LINES -> set_pick_uniforms function ? 6 functions ?
void		set_uniforms(t_env *env, t_uniforms shader_u, t_go_node *node, \
	t_mat4x4 *m)
{
	t_shader	shader;
	t_vec3		view;

	if (shader_u & PICK_SHADER_UNIFORMS)
	{
		shader = env->pick_shader;
		glUniform3fv(shader.u_loc[2], 1, &node->go->pick_clr.x);
	}
	else
	{
		view = vector_rot(env->camera.transform.rotation, (t_vec3)VEC3_FRONT);
		if (shader_u & DEF_SHADER_UNIFORMS)
		{
			shader = env->def_shader;
			set_def_uniforms(shader, env->light, 0.9);//
				// , env->texture_set[env->current_tex], env->def_fade);
		}
		else if (shader_u & STD_SHADER_UNIFORMS)
		{
			shader = env->std_shader;
			set_std_uniforms(shader, env->light, \
				get_mtl(env->mtl_list, node->go->mtl_id));
		}
		glUniform3fv(shader.u_loc[2], 1, &view.x);
	}
	glUniformMatrix4fv(shader.u_loc[0], 1, GL_FALSE, &(m[node->id].m[0]));
	glUniformMatrix4fv(shader.u_loc[1], 1, GL_FALSE, &(m[0].m[0]));
}

void        get_uniforms(t_shader *shdr, t_uniforms shader_u)
{
	shdr->u_loc[0] = glGetUniformLocation(shdr->prog, "m");
	shdr->u_loc[1] = glGetUniformLocation(shdr->prog, "vp");
	if (shader_u & PICK_SHADER_UNIFORMS)
		shdr->u_loc[2] = glGetUniformLocation(shdr->prog, "pickClr");
	else
	{
		shdr->u_loc[2] = glGetUniformLocation(shdr->prog, "viewPos");
		shdr->u_loc[3] = glGetUniformLocation(shdr->prog, "light.position");
		shdr->u_loc[4] = glGetUniformLocation(shdr->prog, "light.color");
		shdr->u_loc[5] = glGetUniformLocation(shdr->prog, "light.intensity");
		shdr->u_loc[6] = glGetUniformLocation(shdr->prog, "light.range");
		if (shader_u & DEF_SHADER_UNIFORMS)
		{
			shdr->u_loc[7] = glGetUniformLocation(shdr->prog, "fade");
			//shdr->u_loc[8] = glGetUniformLocation(shdr->prog, "TextureSampler");
		}
		else if (shader_u & STD_SHADER_UNIFORMS)
		{
			shdr->u_loc[7] = glGetUniformLocation(shdr->prog, "mat.clr_amb");
			shdr->u_loc[8] = glGetUniformLocation(shdr->prog, "mat.clr_dif");
			shdr->u_loc[9] = glGetUniformLocation(shdr->prog, "mat.clr_spc");
			shdr->u_loc[10] = glGetUniformLocation(shdr->prog, "mat.expnt_spc");
			shdr->u_loc[11] = glGetUniformLocation(shdr->prog, "mat.transp");
		}
	}
}