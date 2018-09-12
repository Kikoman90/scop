/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 20:16:12 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	clean_scop(t_env *env)
{
	SDL_GL_DeleteContext(env->gl_context);
	SDL_DestroyWindow(env->window);
	free(env->parser);
	free(env);
	env = NULL;
}

void	loop(t_env *env)
{
	SDL_Event	event;

	while (env->loop == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
		// DrawTriangle();
		SDL_GL_SwapWindow(env->window);
	}
}

int		main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
	{
		printf("delme: SYSTEM FAILURE\n"); //del me
		return (0);
	}
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);
	loop(env);
	clean_scop(env);
	SDL_Quit();
	return (0);
}
