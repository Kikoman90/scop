/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 13:42:55 by fsidler          ###   ########.fr       */
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

/*t_material	*get_mtl(t_mtl_node *list, unsigned int id)
{
	t_mtl_node	*tmp;

	tmp = list;
	while (tmp != NULL)
	{
		if (id == tmp->id)
			return (tmp->mtl);
		tmp = tmp->next;
	}
	return (NULL);
}*/

t_gameobject	*get_gameobject(t_go_node *list, unsigned int id)
{
	t_go_node	*tmp;

	tmp = list;
	while (tmp != NULL)
	{
		if (id == tmp->id)
			return (tmp->go);
		tmp = tmp->next;
	}
	return (NULL);
}

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

/*void		get_model_matrices(t_go_node *go_list, t_mat4x4 *m)
{
	t_go_node	*tmp;

	tmp = go_list;
	while (tmp)
	{
		//tmp->go->mtl_id = 0;
		m[tmp->id] = go_trs(tmp->go->transform);
		//printf("model_matrix[%d]\n", tmp->id - 1);
		//display_mat4x4(m[tmp->id - 1], "MAT");
		tmp = tmp->next;
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

void		get_uniforms(t_shader *shdr, t_uniforms shader_u)
{
	shdr->u_loc[0] = glGetUniformLocation(shdr->prog, "m");
	shdr->u_loc[1] = glGetUniformLocation(shdr->prog, "vp");
	if (shader_u & PICK_SHADER_UNIFORMS)
		shdr->u_loc[2] = glGetUniformLocation(shdr->prog, "pickClr");
	else
	{
		shdr->u_loc[2] = glGetUniformLocation(shdr->prog, "light.position");
		shdr->u_loc[3] = glGetUniformLocation(shdr->prog, "light.color");
		shdr->u_loc[4] = glGetUniformLocation(shdr->prog, "light.intensity");
		shdr->u_loc[5] = glGetUniformLocation(shdr->prog, "light.range");
		if (shader_u & DEF_SHADER_UNIFORMS)
		{
			shdr->u_loc[6] = glGetUniformLocation(shdr->prog, "fade");
			shdr->u_loc[7] = glGetUniformLocation(shdr->prog, "TextureSampler");
		}
		else if (shader_u & STD_SHADER_UNIFORMS)
		{
			shdr->u_loc[6] = glGetUniformLocation(shdr->prog, "mat.clr_amb");
			shdr->u_loc[7] = glGetUniformLocation(shdr->prog, "mat.clr_dif");
			shdr->u_loc[8] = glGetUniformLocation(shdr->prog, "mat.clr_spc");
			shdr->u_loc[9] = glGetUniformLocation(shdr->prog, "mat.expnt_spc");
			shdr->u_loc[10] = glGetUniformLocation(shdr->prog, "mat.transp");
		}
	}
	printf("LOCATIONS = ");
	for (int i = 0; i < 11; i++)
		printf("%2d ", (int)shdr->u_loc[i]);
	printf("\n");
}

/*static void	set_def_uniforms(t_shader shader, t_light light, float fade) // and a texture
{
	glUniform3fv(shader.u_loc[2], 1, &light.go->transform.position.x);
	glUniform3fv(shader.u_loc[3], 1, &light.light_color.x);
	glUniform1f(shader.u_loc[4], light.intensity);
	glUniform1f(shader.u_loc[5], light.range);
	glUniform1f(shader.u_loc[6], fade);
	// texture uniform
}

static void	set_std_uniforms(t_shader shader, t_light light, t_material *mtl)
{
	glUniform3fv(shader.u_loc[2], 1, &light.go->transform.position.x);
	glUniform3fv(shader.u_loc[3], 1, &light.light_color.x);
	glUniform1f(shader.u_loc[4], light.intensity);
	glUniform1f(shader.u_loc[5], light.range);
	glUniform3fv(shader.u_loc[6], 1, &mtl->clr_amb.x);
	glUniform3fv(shader.u_loc[7], 1, &mtl->clr_dif.x);
	glUniform3fv(shader.u_loc[8], 1, &mtl->clr_spc.x);
	glUniform1f(shader.u_loc[9], mtl->expnt_spc);
	glUniform1f(shader.u_loc[10], mtl->transparency);
}

void		set_uniforms(t_env *env, t_uniforms shader_u, t_go_node *node, \
	t_mat4x4 *m)
{
	t_shader	shader;

	if (shader_u & PICK_SHADER_UNIFORMS)
	{
		shader = env->pick_shader;
		glUniform3fv(shader.u_loc[2], 1, &node->go->pick_clr.x);
	}
	else if (shader_u & DEF_SHADER_UNIFORMS)
	{
		shader = env->def_shader;
		set_def_uniforms(shader, env->light, 0.9f);//, env->texture_set[current_tex]);
	}
	else if (shader_u & STD_SHADER_UNIFORMS)
	{
		shader = env->std_shader;
		set_std_uniforms(shader, env->light, \
			get_mtl(env->mtl_list, node->go->mtl_id));
	}
	glUniformMatrix4fv(shader.u_loc[0], 1, GL_FALSE, m[node->id].m);
	glUniformMatrix4fv(shader.u_loc[1], 1, GL_FALSE, m[0].m);
}

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
		glDisableVertexAttribArray(1); // normals
		glDisableVertexAttribArray(2); // UVs
		glDisableVertexAttribArray(3); // grey color array
		glDrawElements(GL_TRIANGLES, tmp->go->idx_count, GL_UNSIGNED_INT, NULL);
		tmp = tmp->next;
	}
}

static void	draw(t_env *env)
{
	if (env->go_mat_update)
	{
		printf("MAT_UPDATE_NEEDED\n");
		env->go_mat_update = mat_update(&env->go_mat, env->go_count);
	}
	if (env->go_mat)
	{
		env->go_mat[0] = mat4x4_mult(env->proj_mat, compute_view(env->camera));
		get_model_matrices(env->go_list, env->go_mat);
		for (unsigned int i = 0; i < env->go_count + 1; i++)
			display_mat4x4(env->go_mat[i], ft_itoa(i));
		//glBindFramebuffer(GL_FRAMEBUFFER, env->ms_fbo);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw_ms_fbo(env, env->go_mat);
		glBindFramebuffer(GL_FRAMEBUFFER, env->pick_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_pick_fbo(env, env->go_mat);
		handle_picking(env->selection, env->go_list, &env->selection_count);
		// draw_handles();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, env->pick_fbo);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, env->ms_fbo);
		glBlitFramebuffer(0, 0, WIN_W, WIN_H, 0, 0, WIN_W, WIN_H, \
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}*/

/*static void	loop(t_env *env)
{
	SDL_Event	event;

	while (env->loop == 1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//HandleEventsAndInput(){
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
		//}
		//draw(env);
		SDL_GL_SwapWindow(env->window);
	}
}*/

int			main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
		return (0);
	//printf("2\n");
	display_go_list(env->go_list);
	//loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
