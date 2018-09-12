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

t_parser	*init_parser(t_env *env)
{
	if (!((env->parser) = (t_parser*)malloc(sizeof(t_parser))))
		return (log_error_null(MALLOC_ERROR));
	env->parser->fline = 1;
	env->parser->fsize = 0;
	env->parser->data = NULL;
	return (env->parser);
}

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
	return ((void*)1);
}

t_env		*init_scop(t_env *env, int argc, char **argv)
{
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (NULL);
	env->loop = 1;
	if (!init_parser(env))
		return (NULL);
	//init_cam, init serializer
	while (argc-- > 1)
		parse_file(env, argv[argc]);
	return (env);
}
