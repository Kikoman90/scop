/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/09 17:42:07 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void __attribute__((constructor)) begin();
void __attribute__((destructor)) end();


t_mat4x4	look_at(t_vec3 pos, t_vec3 target, t_vec3 up)
{
	t_vec3		xaxis;
	t_vec3		yaxis;
	t_vec3		zaxis;
	t_mat4x4	lookat;
	
	zaxis = vec3_norm(vec3_sub(pos, target));
	xaxis = vec3_norm(vec3_cross(up, zaxis));
	yaxis = vec3_cross(zaxis, xaxis);

	lookat = mat4x4();
	lookat.v[0] = vec4_xyzw(xaxis.x, yaxis.x, zaxis.x, 0);
	lookat.v[1] = vec4_xyzw(xaxis.y, yaxis.y, zaxis.y, 0);
	lookat.v[2] = vec4_xyzw(xaxis.z, yaxis.z, zaxis.z, 0);
	lookat.v[3] = vec4_xyzw(-vec3_dot(xaxis, pos), -vec3_dot(yaxis, pos)\
								, -vec3_dot(zaxis, pos), 1);
	return (lookat);
}

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
	return (view);
	//mat.data[12] = -dot(vec3_init_xyz(view.m[0], view.m[]), position);
	//mat.data[13] = -dot(u, position);
	//mat.data[14] = dot(f, position);
}

/*t_mat4x4	look_at(t_vec3 pos, t_vec3 target, t_vec3 up)
{
	t_vec3		xaxis;
	t_vec3		yaxis;
	t_vec3		zaxis;
	t_mat4x4	lookat;
	
	zaxis = vec3_norm(vec3_sub(pos, target));
	xaxis = vec3_norm(vec3_cross(up, zaxis));
	yaxis = vec3_cross(zaxis, xaxis);

	lookat = mat4x4();
	lookat.v[0] = vec4_xyzw(xaxis.x, yaxis.x, zaxis.x, 0);
	lookat.v[1] = vec4_xyzw(xaxis.y, yaxis.y, zaxis.y, 0);
	lookat.v[2] = vec4_xyzw(xaxis.z, yaxis.z, zaxis.z, 0);
	lookat.v[3] = vec4_xyzw(-vec3_dot(xaxis, pos), -vec3_dot(yaxis, pos)\
								, -vec3_dot(zaxis, pos), 1);
	//transpose ?? apparently opengl is column major
	display_mat4x4(lookat);
	return (lookat);
}*/

/*static void	draw(t_env *env)
{
	t_material	*def_mtl;
	t_go_node	*go_tmp;
	t_mat4x4	mvp;
	t_transform	go_tr;
	GLint 		uniform_loc;

	t_mat4x4	deletemeimview;

	go_tmp = env->go_list;
	def_mtl = env->mtl_list->mtl;
	
	//
	deletemeimview = mat4x4_transpose(compute_view(env->camera));
	//

	display_mat4x4(compute_view(env->camera), "cam look at");
	display_mat4x4(env->proj_mat, "proj no transpose");
	env->proj_mat = mat4x4_transpose(env->proj_mat);
	display_mat4x4(env->proj_mat, "proj transpose");
	//mvp = mat4x4_mult(env->proj_mat, compute_view(env->camera));
	mvp = mat4x4_mult(compute_view(env->camera), env->proj_mat);
	display_mat4x4(mvp, "projectionView matrix no transpose");
	mvp = mat4x4_transpose(mvp);
	display_mat4x4(mvp, "projectionView matrix tranpose");
	while (go_tmp)
	{
		go_tr = go_tmp->go->transform;
		mvp = mat4x4_mult(mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale), compute_view(env->camera));
		mvp = mat4x4_mult(env->proj_mat, mvp);
		//mvp = mat4x4_mult(mvp, mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale));
		//mvp = mat4x4_mult(mat4x4_trs(go_tr.position, go_tr.rotation, go_tr.scale), mvp);		
		//printf("before transpose->\n");
		//display_mat4x4(mvp);
		mvp = mat4x4_transpose(mvp);
		//printf("\nafter transpose->\n");
		//display_mat4x4(mvp);


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
}*/

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
		//draw(env);
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

	t_vec3	vec1 = vec3_xyz(2, 1, 0);
	t_quaternion	q1 = quat_tv(90, (t_vec3)VEC3_UP);


	t_vec3 campos = vec3_xyz(-1, 1, -4);
	t_mat4x4 yop = quat_to_mat4x4(q1);
	t_mat4x4 tr = mat4x4_translate(vec3_xyz(-2, 0, 0));
	t_mat4x4 sc = mat4x4_scale(vec3_xyz(2, 2, 4));
	(void)tr;
	(void)sc;

	display_mat4x4(yop, "yop");
	display_vec3("campos", campos);
	//vec1 = vec3_norm(vec1);
	display_vec3("vec1", vec1);
	t_vec3 res = vec3_mat4x4_prod(campos, yop);
	display_vec3("norm res (campos)", vec3_norm(res));
	res = vec3_mat4x4_prod(vec1, yop); 
	display_vec3("res (vec1)", res);	
	display_vec3("norm res (vec1)", vec3_norm(res));




	clean_scop(env, CLEAN_ALL);
	return (0);
}
