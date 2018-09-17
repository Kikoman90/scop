/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 16:39:59 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 17:12:21 by fsidler          ###   ########.fr       */
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

static void	clean_parser(t_env *env)
{
	if (env->parser)
	{
		env->parser->data = NULL;
		free(env->parser);
		env->parser = NULL;
	}
}

static void	clean_obj_list(t_env *env)
{
	t_go_node	*tmp;
	t_go_node	*tmp2;

	if (env->obj_list)
	{
		tmp = env->obj_list;
		while (tmp)
		{
			tmp2 = tmp->next;
			clean_go_node(tmp);
			env->obj_count--;
			tmp = tmp2;
		}
		env->obj_list = NULL;
	}
}

void		*clean_scop(t_env *env, t_clean_flags f)
{
	if (f & CLEAN_ALL)
	{
		clean_sdl(env);
		clean_parser(env);
		clean_obj_list(env);
	}
	else
	{
		if (f & CLEAN_SDL)
			clean_sdl(env);
		if (f & CLEAN_PARSER)
			clean_parser(env);
	}
	free(env);
	env = NULL;
	return (NULL);
}
// need to test everything with all flags. need to include material cleanup
// LEAK COMMAND
