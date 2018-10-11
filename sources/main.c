/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/11 18:38:22 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void __attribute__((constructor)) begin(); //remove after checking leaks
void __attribute__((destructor)) end(); // remove after checking leaks

t_mat4x4	compute_view(t_camera cam)
{
	t_mat4x4	view;

	view = mat4x4_transpose(quat_to_mat4x4(cam.transform.rotation));
	view.m[12] = -vec3_dot(vec3_v4(view.v[0]), cam.transform.position);
	view.m[13] = -vec3_dot(vec3_v4(view.v[1]), cam.transform.position);
	view.m[14] = -vec3_dot(vec3_v4(view.v[2]), cam.transform.position);
	return (view);
}

static void	draw(t_env *env, unsigned int *nb)
{
	t_go_node	*go_tmp;
	t_material	*def_mtl; //
	t_mat4x4	mvp;
	t_transform	go_tr;
	GLint 		uniform_loc;

	go_tmp = env->go_list;
	def_mtl = env->mtl_list->mtl; //
	glBindFramebuffer(GL_FRAMEBUFFER, env->ms_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mvp = mat4x4_mult(env->proj_mat, compute_view(env->camera));
	while (go_tmp)
	{
		go_tr = go_tmp->go->transform;
		glUseProgram(env->def_shader.prog);
		glBindVertexArray(go_tmp->go->gl_stack->vao);
		glEnableVertexAttribArray(0);
		mvp = mat4x4_mult(mvp, mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale));
		uniform_loc = glGetUniformLocation(env->def_shader.prog, "mvp"); // store locations per shader struct
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, mvp.m);
		uniform_loc = glGetUniformLocation(env->def_shader.prog, "uAlpha"); // store locations per shader struct
		glUniform1f(uniform_loc, 0.2);
		*nb = iclamp((int)*nb, 0, go_tmp->go->idx_count); // this is disgusting
		glDrawElements(GL_TRIANGLES, go_tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		go_tmp = go_tmp->next;
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, env->ms_fbo);
	glBlitFramebuffer(0, 0, WIN_W, WIN_H, 0, 0, WIN_W, WIN_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

static void	loop(t_env *env)
{
	SDL_Event	event;
	unsigned int	nb; // beurk !

	nb = 0;
	// materials and shaders... look into that later
	// once things are stable, simple sdl GUI (GUImp ?)
	while (env->loop == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a)
			nb += 1;
		draw(env, &nb);
		SDL_GL_SwapWindow(env->window);
	}
}

int			main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
		return (0);
	loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
