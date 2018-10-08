/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/08 18:59:59 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void __attribute__((constructor)) begin();
void __attribute__((destructor)) end();

t_mat4x4	compute_view(t_camera cam)
{
	t_mat4x4	view;
	t_vec3		axis;

	view = quat_to_mat4x4(cam.transform.rotation);
	//view = mat4x4_inv(view);
	axis = vec3_xyz(view.m[0], view.m[4], view.m[8]);
	view.m[12] = -vec3_dot(axis, cam.transform.position);
	axis = vec3_xyz(view.m[1], view.m[5], view.m[9]);
	view.m[13] = -vec3_dot(axis, cam.transform.position);
	axis = vec3_xyz(view.m[2], view.m[6], view.m[10]);
	view.m[14] = -vec3_dot(axis, cam.transform.position);// or no sign
	view = mat4x4_transpose(view); // ?
	return (view);
	//mat.data[12] = -dot(vec3_init_xyz(view.m[0], view.m[]), position);
	//mat.data[13] = -dot(u, position);
	//mat.data[14] = dot(f, position);
}

static void	draw(t_env *env)
{
	t_material	*def_mtl;
	t_go_node	*go_tmp;
	t_mat4x4	mvp;
	t_transform	go_tr;
	GLint 		uniform_loc;

	go_tmp = env->go_list;
	def_mtl = env->mtl_list->mtl;
	mvp = mat4x4_mult(env->proj_mat, compute_view(env->camera));
	while (go_tmp)
	{
		go_tr = go_tmp->go->transform;
		mvp = mat4x4_mult(mvp, mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale));

		glUseProgram(env->def_shader.prog);
		
		glBindVertexArray(go_tmp->go->gl_stack->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go_tmp->go->gl_stack->ibo);
		glEnableVertexAttribArray(0);

		uniform_loc = glGetUniformLocation(env->def_shader.prog, "mvp");
		// void glUniformMatrix4fv(	GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, mvp.m);
		uniform_loc = glGetUniformLocation(env->def_shader.prog, "uAlpha");
		glUniform1f(uniform_loc, 0.2);
		
		glDrawElements(GL_TRIANGLES, go_tmp->go->idx_count, GL_UNSIGNED_INT, NULL);

		//glUseProgram(0);
		//glBindVertexArray(0);
		go_tmp = go_tmp->next;
	}
}

static void	loop(t_env *env)
{
	SDL_Event	event;

	// materials and shaders... look into that later
	// once things are stable, simple sdl GUI (GUImp ?)
	while (env->loop == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
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
	// display_go_list(env->go_list);
	// display_mtl_list(env->mtl_list);
	loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
