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

// useful sdl functions
// SDL_SetWindowFullscreen(win, flags); flags = WINDOW_FULLSCREEN / WINDOW_FULLSCREEN_DESKTOP / 0
// SDL_GL_SetSwapInterval(int interval); interval = 0 (immediate update) / 1 (vsync) / -1 (adaptive vsync)
// adaptive vsync seems to be the best but isn't supported on all systems. so if SDL_GL_SetSwapInterval(-1) returns -1 (fail)
// just use SDL_GL_SetSwapInterval(1) instead

/*void	DrawTriangle() {

	float[] vertexPositions = {
		0.75f, 0.75f, 0.0f, 1.0f,
    	0.75f, -0.75f, 0.0f, 1.0f,
    	-0.75f, -0.75f, 0.0f, 1.0f
	};

	GLuint	vao;
		

}*/

//void	readShader(shaderType geo/vtx/frag)



void	loop(t_env *env) {

	SDL_Event 	event;

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			env->loop = 0;
		//DrawTriangle();
		SDL_GL_SwapWindow(env->window);
	} while (env->loop == 1);

}

int		main(int argc, char **argv)
{
	t_env	*env;

	if (!init_scop(&env, argc, argv))
		return (0);
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);

	// main loop
	loop(env);

	// cleanup
	SDL_GL_DeleteContext(env->glContext);
	SDL_DestroyWindow(env->window);
	// free parser
	free(env);
	SDL_Quit();

	return (0);
}