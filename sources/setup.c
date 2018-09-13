/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 21:18:49 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		*init_sdl_gl(t_env *env)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (log_error_null(SDL_INIT_ERROR));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, \
		SDL_GL_CONTEXT_PROFILE_CORE);
	if (!(env->window = SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE \
		| SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED)))
		return (log_error_null(WIN_CREATE_ERROR));
	if (!(env->gl_context = SDL_GL_CreateContext(env->window)))
		return (log_error_null(SDL_GetError()));
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);
	return ((void*)1);
}

t_env		*init_scop(t_env *env, int argc, char **argv)
{
	// init_null -> window = NULL, gl_context = null, parser = null ??????? IS IT NECESSARY ?
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (clean_scop(env, CLEAN_SDL));
	if (!((env->parser) = (t_parser*)malloc(sizeof(t_parser))))
	{
		clean_scop(env, CLEAN_SDL);
		return (log_error_null(MALLOC_ERROR));
	}
	//init_cam, init serializer
	env->obj_list = NULL;
	// this need to be its own function
	while (argc-- > 1)
	{
		if (!parse_file(env, argv[argc]))
			return (clean_scop(env, CLEAN_SDL | CLEAN_PARSER));
	}
	// this need to be its own function
	env->loop = 1;
	return (env);
}

// clean(env, CLEAN_SDL | CLEAN_PARSER | CLEAN_OBJ_LIST);
// clean(env, CLEAN_ALL);
// THIS IS WHAT I WANT
