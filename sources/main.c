/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/02 19:48:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void __attribute__((constructor)) begin();
void __attribute__((destructor)) end();

// write draw function here, fill in uniforms, bind vao etc.

static void	loop(t_env *env)
{
	SDL_Event	event;

	// send uniforms and bind vaos in loop
	// materials and shaders... look into that later
	// once things are stable, simple sdl GUI (GUImp ?)
	while (env->loop == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
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
	display_mtl_list(env->mtl_list);
	loop(env);
	clean_scop(env, CLEAN_ALL);
	return (0);
}
