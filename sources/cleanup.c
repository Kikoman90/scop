/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 16:39:59 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 16:31:50 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void			clear_mtl_list(t_mtl_list *list, int free_mtl)
{
	t_mtl_node	*tmp;
	t_mtl_node	*tmp_next;

	if (list->head)
	{
		tmp = list->head;
		while (tmp)
		{
			tmp_next = tmp->next;
			clean_mtl_node(tmp, free_mtl);
			list->count--;
			tmp = tmp_next;
		}
		list->head = NULL;
	}
}

void			clear_go_list(t_go_list *list, int free_go)
{
	t_go_node	*tmp;
	t_go_node	*tmp_next;

	if (list)
	{
		tmp = list->head;
		while (tmp)
		{
			tmp_next = tmp->next;
			clean_go_node(tmp, free_go);
			list->count--;
			tmp = tmp_next;
		}
		list->head = NULL;
	}
}

void			clear_tr_list(t_tr_list *list)
{
	t_tr_node	*tmp;
	t_tr_node	*tmp_next;

	if (list)
	{
		tmp = list->head;
		while (tmp)
		{
			tmp_next = tmp->next;
            tmp->next = NULL;
			free(tmp);
            tmp = NULL;
			list->count--;
			tmp = tmp_next;
		}
		list->head = NULL;
	}
}

static void		clean_shaders_and_primitives(t_shader *shaders, \
	t_geometry *primitives, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (shaders[i].prog > 0)
			glDeleteProgram(shaders[i].prog);
		if (shaders[i].name)
			free(shaders[i].name);
		if (primitives[i].name)
			free(primitives[i].name);
		glDeleteBuffers(1, &primitives[i].vbo);
		glDeleteVertexArrays(1, &primitives[i].vao);
		i++;
	}
}

unsigned int	clean_scop(t_env *env)
{
	if (env->matrices.model)
		free(env->matrices.model);
	clear_tr_list(&env->selection.list);
	clear_mtl_list(&env->materials, 1);
	clear_go_list(&env->gameobjects, 1);
	glDeleteTextures(4, &env->skyboxes[0]);
	glDeleteTextures(7, &env->textures[0]);
	clean_shaders_and_primitives(env->shaders, env->primitives, 6);
	glDeleteRenderbuffers(4, &env->buffers.rbo[0]);
	if (env->buffers.ms_fbo > 0)
		glDeleteFramebuffers(1, &env->buffers.ms_fbo);
	if (env->buffers.pick_fbo > 0)
		glDeleteFramebuffers(1, &env->buffers.pick_fbo);
	if (env->win_env.gl_context)
		SDL_GL_DeleteContext(env->win_env.gl_context);
	if (env->win_env.window)
		SDL_DestroyWindow(env->win_env.window);
	return (0);
}
