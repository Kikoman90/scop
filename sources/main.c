/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/17 17:56:48 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

//void __attribute__((constructor)) begin(); //remove after checking leaks
//void __attribute__((destructor)) end() {}; // remove after checking leaks

t_mat4x4	compute_view(t_camera cam)
{
	t_mat4x4	view;

	view = mat4x4_transpose(quat_to_mat4x4(cam.transform.rotation));
	view.m[12] = -vec3_dot(vec3_v4(view.v[0]), cam.transform.position);
	view.m[13] = -vec3_dot(vec3_v4(view.v[1]), cam.transform.position);
	view.m[14] = -vec3_dot(vec3_v4(view.v[2]), cam.transform.position);
	return (view);
}

static void	draw(t_env *env, unsigned int *nb, int x, int y, int lmb)
{
	t_go_node	*go_tmp;
	//t_material	*def_mtl; //
	t_mat4x4	mvp;
	t_mat4x4	vp;
	t_transform	go_tr;
	GLint 		uniform_loc;

	unsigned char data[3];
	unsigned int pickedID;


	go_tmp = env->go_list;
	//def_mtl = env->mtl_list->mtl; //
	glBindFramebuffer(GL_FRAMEBUFFER, env->pick_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, env->ms_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vp = mat4x4_mult(env->proj_mat, compute_view(env->camera));
	(void)nb;
	while (go_tmp)
	{
		if (ft_strcmp(go_tmp->go->name, "MameneMamene") == 0)
			go_tmp->go->transform.position.x += 0.001f;
		go_tr = go_tmp->go->transform;
		
		glBindFramebuffer(GL_FRAMEBUFFER, env->ms_fbo);
		glUseProgram(env->def_shader.prog);
		glBindVertexArray(go_tmp->go->gl_stack->vao);
		mvp = mat4x4_mult(vp, mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale));
		uniform_loc = glGetUniformLocation(env->def_shader.prog, "mvp"); // store locations per shader struct
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, mvp.m);
		uniform_loc = glGetUniformLocation(env->def_shader.prog, "uAlpha"); // store locations per shader struct
		glUniform1f(uniform_loc, 0.2);
		*nb = ft_iclamp((int)*nb, 0, go_tmp->go->idx_count); // this is disgusting
		glDrawElements(GL_TRIANGLES, go_tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		
		glBindFramebuffer(GL_FRAMEBUFFER, env->pick_fbo);
		glUseProgram(env->pick_shader.prog);
		uniform_loc = glGetUniformLocation(env->pick_shader.prog, "mvp"); // store locations per shader struct
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, mvp.m);
		uniform_loc = glGetUniformLocation(env->pick_shader.prog, "uPickColor");
		glUniform3fv(uniform_loc, 1, &go_tmp->go->pick_clr.v[0]);
		glDrawElements(GL_TRIANGLES, go_tmp->go->idx_count, GL_UNSIGNED_INT, NULL);

		go_tmp = go_tmp->next;
	}
	if (lmb == 1)
	{
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
		//printf("%f %f %f\n", (float)data[0] / 255.0f, (float)data[1] / 255.0f, (float)data[2] / 255.0f);
		pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;
		printf("PICKED ID: %d\n", pickedID);
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, env->ms_fbo);
	glBlitFramebuffer(0, 0, WIN_W, WIN_H, 0, 0, WIN_W, WIN_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

static void	loop(t_env *env)
{
	SDL_Event	event;
	unsigned int	nb; // beurk !
	int	x;
	int	y;
	int	lmb;

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
		
		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1))
			lmb = 1;
		else
			lmb = 0;
		//lmb = SDL_BUTTON(SDL_BUTTON_LEFT) ? 0 : 1;
		draw(env, &nb, x, WIN_H - 1 - y, lmb);
		SDL_GL_SwapWindow(env->window);
	}
}

int			main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
		return (0);
	display_go_list(env->go_list);
	display_mtl_list(env->mtl_list);
	loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
