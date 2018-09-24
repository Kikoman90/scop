/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 16:39:59 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/18 14:04:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	clean_sdl(t_env *env)
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
	SDL_Quit();
}

static void	clean_mtl_list(t_env *env)
{
	t_mtl_node	*tmp;
	t_mtl_node	*tmp2;

	if (env->mtl_list)
	{
		tmp = env->mtl_list;
		while (tmp)
		{
			tmp2 = tmp->next;
			clean_mtl_node(tmp);
			env->mtl_count--;
			tmp = tmp2;
		}
		env->mtl_list = NULL;
	}
}

static void	clean_go_list(t_env *env)
{
	t_go_node	*tmp;
	t_go_node	*tmp2;

	if (env->go_list)
	{
		tmp = env->go_list;
		while (tmp)
		{
			tmp2 = tmp->next;
			clean_go_node(tmp);
			env->go_count--;
			tmp = tmp2;
		}
		env->go_list = NULL;
	}
}

void		*clean_scop(t_env *env, t_clean_flags f)
{
	if (f & CLEAN_ALL)
	{
		clean_sdl(env);
		clean_go_list(env);
		clean_mtl_list(env);
	}
	else if (f & CLEAN_SDL)
	{
		clean_sdl(env);
	}
	free(env);
	env = NULL;
	return (NULL);
}
