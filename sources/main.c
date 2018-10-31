/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:56:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

//void __attribute__((constructor)) begin(); //remove after checking leaks
//void __attribute__((destructor)) end() {}; // remove after checking leaks

/*void		add_to_selection(t_go_node *selection, t_go_node *go_list, \
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

/*void		handle_picking(t_go_node *selection, t_go_node *go_list, \
	size_t *count)
{
	int				x;
	int				y;
	unsigned int	picked_id;
	unsigned char	data[3];

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1))
	{
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
		picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
		// if hold shift -> add
		// else clear, and add (if picked_id != 0)
		if (picked_id != 0)
			add_to_selection(selection, go_list, count, picked_id);
	}
}*/

void		draw_ms_fbo(t_env *env, t_mat4x4 *m)
{
	t_go_node	*tmp;
	
	tmp = env->go_list;
	while (tmp)
	{
		glBindVertexArray(tmp->go->gl_stack->vao);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		if (tmp->go->mtl_id != 0)
		{
			glUseProgram(env->std_shader.prog);
			set_uniforms(env, STD_SHADER_UNIFORMS, tmp, m);
			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(4);
		}
		else
		{
			glUseProgram(env->def_shader.prog);
			set_uniforms(env, DEF_SHADER_UNIFORMS, tmp, m);
		}
		glDrawElements(GL_TRIANGLES, tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		tmp = tmp->next;
	}
	// draw_world_axes_and_grid(env);
}

void		draw_pick_fbo(t_env *env, t_mat4x4 *m)
{
	t_go_node	*tmp;

	tmp = env->go_list;
	while (tmp)
	{
		glUseProgram(env->pick_shader.prog);
		set_uniforms(env, PICK_SHADER_UNIFORMS, tmp, m);
		glBindVertexArray(tmp->go->gl_stack->vao);
		glEnableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDrawElements(GL_TRIANGLES, tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		tmp = tmp->next;
	}
}

static void	draw(t_env *env)
{
	if (env->go_mat_update)
		env->go_mat_update = mat_update(&env->go_mat, env->go_count);
	if (env->go_mat)
	{
		env->go_mat[0] = mat4x4_mult(env->proj_mat, compute_view(env->camera));
		get_model_matrices(env->go_list, env->go_mat);
		glBindFramebuffer(GL_FRAMEBUFFER, env->ms_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_ms_fbo(env, env->go_mat);
		glBindFramebuffer(GL_FRAMEBUFFER, env->pick_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_pick_fbo(env, env->go_mat);
		// handle_picking(env->selection, env->go_list, &env->selection_count);
		// draw_handles(); // no depth test ?
		// draw_mini_axes(env) // no depth test !
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, env->ms_fbo);
		glBlitFramebuffer(0, 0, WIN_W, WIN_H, 0, 0, WIN_W, WIN_H, \
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

static void	loop(t_env *env)
{
	uint32_t	last_time;
	uint32_t	cur_time;
	double		delta_time;

	cur_time = SDL_GetTicks();
	while (env->loop == 1)
	{
		last_time = cur_time;
		cur_time = SDL_GetTicks();
		delta_time = (cur_time - last_time) / 1000.0;
		// SDL_Delay(1.0f/60.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rotate_gameobjects(env->go_list, delta_time);
		handle_events_and_input(env);
		draw(env);
		SDL_GL_SwapWindow(env->window);
	}
}

int			main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
		return (0);
	//display_go_list(env->go_list);
	//glBindVertexArray(0); // ?
	loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
