/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/14 15:05:20 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	loop(t_env *env)
{
	SDL_Event	event;

	while (env->loop == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
		// DrawObjList(env); ou DrawObjList(env->obj_list, env->camera); ...
		// draw by material (material affinities... high-lvl low-lvl) ...
		SDL_GL_SwapWindow(env->window);
	}
}

int			main(int argc, char **argv)
{
	t_env	*env;

	env = NULL;
	if (!(env = init_scop(env, argc, argv)))
		return (0);
	display_go_list(env->go_list);
	loop(env);
	clean_scop(env, CLEAN_ALL);
	//keep looping and see if clean_scop leaves no leaks.
	return (0);
}
