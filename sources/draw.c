/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 17:43:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:49:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	draw_ms_fbo(t_env *env)
{
	t_go_node	*tmp;

	tmp = env->gameobjects.head;
	while (tmp)
	{
		glBindVertexArray(tmp->go->gl_stack.vao);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		if (tmp->go->mtl_id != 0)
		{
			glUseProgram(env->shaders[2].prog);
			set_uniforms(env, tmp, 2);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
		}
		else
		{
			glUseProgram(env->shaders[0].prog);
			set_uniforms(env, tmp, 0);
		}
		glDrawElements(GL_TRIANGLES, tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		tmp = tmp->next;
	}
	// draw_world_axes_and_grid(env);
}

static void	draw_pick_fbo(t_env *env)
{
	t_go_node	*tmp;

	tmp = env->gameobjects.head;
	while (tmp)
	{
		glUseProgram(env->shaders[1].prog);
		set_uniforms(env, tmp, 1);
		glBindVertexArray(tmp->go->gl_stack.vao);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDrawElements(GL_TRIANGLES, tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		tmp = tmp->next;
	}
}

void		draw(t_env *env)
{
	update_matrices(env);
	if (env->matrices.model && env->gameobjects.count > 0)
	{
		get_model_matrices(env->gameobjects.head, env->matrices.model);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.ms_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_ms_fbo(env);
		glBindFramebuffer(GL_FRAMEBUFFER, env->buffers.pick_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_pick_fbo(env);
		// handle_picking(env->selection, env->go_list, &env->selection_count);
		// draw_handles(); // no depth test ?
		// draw_mini_axes(env) // no depth test !
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.ms_fbo);
		glBlitFramebuffer(0, 0, env->win_env.win_w, env->win_env.win_h, \
			0, 0, env->win_env.win_w, env->win_env.win_h, \
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}
