/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/11 12:56:46 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <stdio.h> //rem me

t_env	*init_scop(t_env **env, int argc, char **argv)
{
	int	i;

	// init struct
	if (!((*env) = (t_env*)malloc(sizeof(t_env))))
		return (LogErrorNull(MALLOC_ERROR));

	// init sdl/opengl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (LogErrorNull(SDL_INIT_ERROR));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, \
		SDL_GL_CONTEXT_PROFILE_CORE);
	if (!((*env)->window = SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE \
		| SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED)))
		return (LogErrorNull(WIN_CREATE_ERROR));
	(*env)->glContext = SDL_GL_CreateContext((*env)->window);

	// init loop, carmera, objList
	(*env)->loop = 1;

	//init_cam(...);
	//init_parser(...);
	//init_serializer(...);

	i = 0;
	while (++i < argc)
	{
		printf("%d\n", i); // remove stdio and printf, check all 42 headers, norminette on all .c .h files of all folders...
		parseFile(argv[i]);
	}
	return (*env);
}