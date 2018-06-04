/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/05/30 19:18:27 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	loop(t_env *env) {

	SDL_Event 	event;

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;

		SDL_GL_SwapWindow(env->sdl->win);
	} while (env->loop == 1);

}

t_env	*init_env(char **argv)
{
	t_env	*env;

	//
	(void)argv;
	//

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (LogErrorNull(MALLOC_ERROR));
	
	if (!(env->sdl = (t_sdl*)malloc(sizeof(t_sdl))))
		return (LogErrorNull(MALLOC_ERROR));
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (LogErrorNull(SDL_INIT_ERROR));
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	if (!(env->sdl->win = SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, \
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED)))
		return (LogErrorNull(WIN_CREATE_ERROR));
	env->sdl->glContext = SDL_GL_CreateContext(env->sdl->win);

	env->loop = 1;

	return (env);
}

int		main(int argc, char **argv)
{
	t_env	*env;

	//
	(void)argc;
	(void)argv;
	//

	if (!(env = init_env(argv)))
		return (0);
	glClearColor(0.19, 0.27, 0.41, 1);

	// main loop
	loop(env);

	// cleanup
	SDL_GL_DeleteContext(env->sdl->glContext);
	SDL_DestroyWindow(env->sdl->win);
	// free parser
	free(env);

	SDL_Quit();

	return (0);
}