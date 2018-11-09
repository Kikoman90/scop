/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 17:43:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/09 20:28:37 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	draw_ms_fbo(t_env *env)
{
	t_go_node	*tmp;

	tmp = env->gameobjects.head;
	while (tmp)
	{
		glBindVertexArray(tmp->go->vao);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		tmp->go->mtl_id = 0; // HAHAHAHAHAHA
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
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
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
		glBindVertexArray(tmp->go->vao);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDrawArrays(GL_TRIANGLES, 0, tmp->go->vtx_count);
		tmp = tmp->next;
	}
}

/*void		add_to_selection(t_go_list *selection, t_go_node *go_list, \
	size_t *count, unsigned int id)
{
	t_go_node	*tmp;

	tmp = selection;
	if (tmp == NULL && (selection = get_go_node(go_list, id)))
		*count += 1;
	else
	{
		while (tmp->next != NULL)
		{
			if (id == tmp->id)
				return ;
			tmp = tmp->next;
		}
		if (id != tmp->id && (tmp->next = get_go_node(go_list, id)))
			*count += 1;
	}
}*/

// rect_selection ?

// create_primitive(type, param**)
// create_primitive(cone, height, radius)
// create_primitive(arrow, ...)
// create_primitive(cylinder);
// return a gl_stack;
// also indicate number of subdivisions for the primitive construction

/*void		handle_picking(t_go_list *selection, t_go_list *gameobjects)
{
	int				x;
	int				y;
	unsigned int	picked_id;
	unsigned char	data[3];

	(void)selection;
	(void)gameobjects;
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1)) // key and button states in struct
	{
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_INT, data);
		picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
		// hold shift manage
		if (picked_id != 0)
		{
			printf("picked id = %u\n", picked_id);
			//add_to_selection(selection, gameobjects, picked_id);
		}
	}
}

static void	draw_handles(t_go_list *selection)
{
	t_go_node	*tmp;
	t_vec3		handle_pos;

	handle_pos = (t_vec3)VEC3_ZERO;
	tmp = selection->head;
	while (tmp)
	{
		handle_pos = vec3_add(handle_pos, tmp->go->transform.position);
		tmp = tmp->next;
	}
	handle_pos = vec3_scale(handle_pos, 1.0f / (float)selection->count);
	//drawCone(..., handle_pos);
}*/

void		draw(t_env *env)
{
	update_matrices(env, -1);
	if (env->matrices.model && env->gameobjects.count > 0)
	{
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
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}
