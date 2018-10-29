/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 16:39:59 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/26 17:19:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	clean_sdl_gl(t_env *env)
{
	if (env->gl_context)
	{
		SDL_GL_DeleteContext(env->gl_context);
		env->gl_context = NULL;
	}
	if (env->window)
	{
		SDL_DestroyWindow(env->window);
		env->window = NULL;
	}
	if (env->ms_fbo != 0)
		glDeleteFramebuffers(1, &env->ms_fbo);
	if (env->pick_fbo != 0)
		glDeleteFramebuffers(1, &env->pick_fbo);
	if (env->def_shader.prog != 0)
		glDeleteProgram(env->def_shader.prog);
	if (env->pick_shader.prog != 0)
		glDeleteProgram(env->pick_shader.prog);
	if (env->std_shader.prog != 0)
		glDeleteProgram(env->std_shader.prog);
	SDL_Quit();
}

static void	clean_mtl_list(t_mtl_node *list, size_t *count, unsigned int no_f)
{
	t_mtl_node	*tmp;
	t_mtl_node	*tmp2;

	if (list)
	{
		tmp = list;
		while (tmp)
		{
			tmp2 = tmp->next;
			clean_mtl_node(tmp, no_f);
			*count -= 1;
			tmp = tmp2;
		}
		list = NULL;
	}
}

static void	clean_go_list(t_go_node *list, size_t *count, unsigned int no_f)
{
	t_go_node	*tmp;
	t_go_node	*tmp2;

	if (list)
	{
		tmp = list;
		while (tmp)
		{
			tmp2 = tmp->next;
			clean_go_node(tmp, no_f);
			*count -= 1;
			tmp = tmp2;
		}
		list = NULL;
	}
}

void		*clean_scop(t_env *env, t_clean_flags f)
{
	if (f & CLEAN_ALL)
	{
		clean_sdl_gl(env);
		clean_go_list(env->selection, &env->selection_count, 1);
		clean_go_list(env->go_list, &env->go_count, 0);
		clean_mtl_list(env->mtl_list, &env->mtl_count, 0);
		if (env->light.go)
			free(env->light.go);
		if (env->go_mat)
			free(env->go_mat);
	}
	else if (f & CLEAN_SDL_GL)
	{
		clean_sdl_gl(env);
	}
	free(env);
	env = NULL;
	return (NULL);
}
