/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 17:43:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/19 20:53:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLuint	get_primitive_vao(t_geometry (*primitives)[6], size_t *count, \
	const char *name)
{
	unsigned int	i;

	i = 0;
	while (i < 6)
	{
		if ((*primitives)[i].name && ft_strcmp((*primitives)[i].name, name) == 0)
		{
			*count = (*primitives)[i].count;
			return ((*primitives)[i].vao);
		}
		i++;
	}
	*count = 0;
	return (0);
}

static void	draw_handles(t_env *env)
{
	t_shader	shader;
	size_t	count;
	GLuint	vao;

	glDisable(GL_DEPTH_TEST);
	shader = env->shaders[get_shader_idx(&env->shaders, "primitive")];
	glUseProgram(shader.prog);
	selection_transform(&env->selection);
	set_uniforms(env, &shader, NULL);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	if (env->selection.mode & SCOP_TRANSLATE)
	{
		vao = get_primitive_vao(&env->primitives, &count, "cone");
		glBindVertexArray(vao);
		//
		env->selection.rot[0] = mat4x4_trs(vec3_xyz(8, 0, 0), quat_tv(90, (t_vec3)VEC3_FRONT), (t_vec3)VEC3_ONE);
		env->selection.rot[1] = mat4x4_trs(vec3_xyz(0, 8, 0), quat(), (t_vec3)VEC3_ONE);
		env->selection.rot[2] = mat4x4_trs(vec3_xyz(0, 0, 8), quat_tv(-90, (t_vec3)VEC3_RIGHT), (t_vec3)VEC3_ONE);
		glUniformMatrix4fv(shader.u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
		env->selection.colors[0] = vec3_norm(vec3_xyz(0.9f, 0.1f, 0.1f));
		env->selection.colors[1] = vec3_norm(vec3_xyz(0.1f, 0.9f, 0.1f));
		env->selection.colors[2] = vec3_norm(vec3_xyz(0.1f, 0.1f, 0.9f));
		env->selection.colors[3] = vec3_norm(vec3_xyz(0.45f, 0.45f, 0.1f));
		glUniform3fv(shader.u_loc[4], 4, &env->selection.colors[0].x);
		//
		glDrawArraysInstanced(GL_TRIANGLES, 0, count, 3);

		vao = get_primitive_vao(&env->primitives, &count, "line");
		glBindVertexArray(vao);

		//
		env->selection.rot[0] = mat4x4_trs(vec3_xyz(4, 0, 0), quat_tv(90, (t_vec3)VEC3_FRONT), (t_vec3)VEC3_ONE);
		env->selection.rot[1] = mat4x4_trs(vec3_xyz(0, 4, 0), quat(), (t_vec3)VEC3_ONE);
		env->selection.rot[2] = mat4x4_trs(vec3_xyz(0, 0, 4), quat_tv(-90, (t_vec3)VEC3_RIGHT), (t_vec3)VEC3_ONE);
		glUniformMatrix4fv(shader.u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
		env->selection.colors[0] = vec3_norm(vec3_xyz(0.9f, 0.1f, 0.1f));
		env->selection.colors[1] = vec3_norm(vec3_xyz(0.1f, 0.9f, 0.1f));
		env->selection.colors[2] = vec3_norm(vec3_xyz(0.1f, 0.1f, 0.9f));
		env->selection.colors[3] = vec3_norm(vec3_xyz(0.45f, 0.45f, 0.1f));
		glUniform3fv(shader.u_loc[4], 4, &env->selection.colors[0].x);
		//
		glDrawArraysInstanced(GL_LINES, 0, count, 3);

	}
	else if (env->selection.mode & SCOP_ROTATE)
	{
		
	}
	else if (env->selection.mode & SCOP_SCALE)
	{

	}
	glEnable(GL_DEPTH_TEST);
}

static void	draw_ms_fbo(t_env *env)
{
	t_go_node	*tmp;
	int			shdr_idx;
	size_t		count;
	GLuint		p_vao;

	tmp = env->gameobjects.head;
	while (tmp)
	{
		glBindVertexArray(tmp->go->vao);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		if (tmp->go->mtl_id != 0)
		{
			shdr_idx = get_shader_idx(&env->shaders, "standard");
			glUseProgram(env->shaders[shdr_idx].prog);
			set_uniforms(env, &env->shaders[shdr_idx], tmp);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
		}
		else
		{
			shdr_idx = get_shader_idx(&env->shaders, "default");
			glUseProgram(env->shaders[shdr_idx].prog);
			set_uniforms(env, &env->shaders[shdr_idx], tmp);
		}
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
		tmp = tmp->next;
	}
	//
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	shdr_idx = get_shader_idx(&env->shaders, "skybox");
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glUseProgram(env->shaders[shdr_idx].prog);
	set_uniforms(env, &env->shaders[shdr_idx], NULL);
	p_vao = get_primitive_vao(&env->primitives, &count, "cube");
	glBindVertexArray(p_vao);
	glDrawArrays(GL_TRIANGLES, 0, count);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	//
	//glEnable()
	shdr_idx = get_shader_idx(&env->shaders, "gizmos");
	glUseProgram(env->shaders[shdr_idx].prog);
	set_uniforms(env, &env->shaders[shdr_idx], NULL);
	p_vao = get_primitive_vao(&env->primitives, &count, "plane");
	glBindVertexArray(p_vao);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_TRIANGLES, 0, count);

	//
	if (env->selection.list.count > 0)
		draw_handles(env);
	//
	// draw_world_axes_and_grid(env);
}

static void	draw_pick_fbo(t_env *env)
{
	t_go_node	*tmp;
	int			shdr_idx;
	size_t		count;
	GLuint		p_vao;

	tmp = env->gameobjects.head;
	shdr_idx = get_shader_idx(&env->shaders, "pick");
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	while (tmp)
	{
		glUseProgram(env->shaders[shdr_idx].prog);
		set_uniforms(env, &env->shaders[shdr_idx], tmp);
		glBindVertexArray(tmp->go->vao);
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
		tmp = tmp->next;
	}
	shdr_idx = get_shader_idx(&env->shaders, "pick");
	glUseProgram(env->shaders[shdr_idx].prog);
	set_uniforms(env, &env->shaders[shdr_idx], NULL);
	p_vao = get_primitive_vao(&env->primitives, &count, "plane");
	glBindVertexArray(p_vao);
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void		draw(t_env *env)
{
	update_matrices(env, -1);
	if (env->matrices.model && env->gameobjects.count > 0)
	{
		glDepthFunc(GL_LESS);
		get_model_matrices(env->gameobjects.head, env->matrices.model);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.ms_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_ms_fbo(env);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.pick_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_pick_fbo(env);
		//handle_picking(&env->selection, &env->gameobjects);
		//draw_handles(&env->selection); // no depth test ?
		// draw_mini_axes(env) // no depth test !
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.ms_fbo);
		glBlitFramebuffer(0, 0, env->win_env.win_w, env->win_env.win_h, \
			0, 0, env->win_env.win_w, env->win_env.win_h, \
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
}
