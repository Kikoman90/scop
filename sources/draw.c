/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 17:43:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 13:28:46 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	draw_skybox(t_env *env, t_shader *shader)
{
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glUseProgram(shader->prog);
	set_uniforms(env, shader, NULL);
	glBindVertexArray(env->primitives[SCOP_CUBE].vao);
	glDrawArrays(GL_TRIANGLES, 0, env->primitives[SCOP_CUBE].count);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

static void	draw_gizmos(t_env *env, t_shader *shader)
{
	glUseProgram(shader->prog);
	set_uniforms(env, shader, NULL);
	glBindVertexArray(env->primitives[SCOP_PLANE].vao);
	glDrawArrays(GL_TRIANGLES, 0, env->primitives[SCOP_PLANE].count);
}

static void	draw_ms_fbo(t_env *env)
{
	t_go_node	*tmp;
	t_shader	shader;

	tmp = env->gameobjects.head;
	while (tmp)
	{
		glBindVertexArray(tmp->go->vao);
		shader = (tmp->go->mtl_id) ? env->shaders[STANDARD_SHADER] : \
			env->shaders[DEFAULT_SHADER];
		glUseProgram(shader.prog);
		set_uniforms(env, &shader, tmp);
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
		tmp = tmp->next;
	}
	draw_skybox(env, &env->shaders[SKYBOX_SHADER]);
	draw_gizmos(env, &env->shaders[BILLBOARD_SHADER]);
	if (env->selection.list.count > 0)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		draw_handles(env, &env->shaders[PRIMITIVE_SHADER], \
			vec3_length(vec3_sub(env->selection.transform.position, \
			env->camera.transform.position)) * (135.0f / env->win_env.win_h));
	}
}

static void	draw_pick_fbo(t_env *env)
{
	t_go_node	*tmp;
	t_shader	shader;

	shader = env->shaders[PICK_SHADER];
	glUseProgram(shader.prog);
	tmp = env->gameobjects.head;
	while (tmp)
	{
		set_uniforms(env, &shader, tmp);
		glBindVertexArray(tmp->go->vao);
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
		tmp = tmp->next;
	}
	set_uniforms(env, &shader, NULL);
	glBindVertexArray(env->primitives[SCOP_PLANE].vao);
	glDrawArrays(GL_TRIANGLES, 0, env->primitives[SCOP_PLANE].count);
}

void		draw(t_env *env)
{
	update_matrices(env, -1);
	selection_transform(&env->selection);
	if (env->matrices.model && env->gameobjects.count > 0)
	{
		get_model_matrices(env->gameobjects.head, env->matrices.model);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.ms_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_ms_fbo(env);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.pick_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_pick_fbo(env);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.ms_fbo);
		glBlitFramebuffer(0, 0, env->win_env.win_w, env->win_env.win_h, \
			0, 0, env->win_env.win_w, env->win_env.win_h, \
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
}
