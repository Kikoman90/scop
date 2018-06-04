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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

void get_argv(char** argv) {
	printf("YAY AND %s\n", argv[1]);
}

void loopStopper() {

	int f = 1;
	SDL_Event event;

	do
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
			printf("DONEEEE!\n");
			f = 0;
		}
	} while (f == 1);

}

int main(int argc, char **argv)
{
	if (argc >= 2)
	{
		get_argv(argv);
		// start program with arguments : check if files exist, try reading and parsing it.
	}
	else
	{
		// start program without arguments
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
		return (1);
	}
	// here

	SDL_Window *win;

	win = SDL_CreateWindow("SCOP", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | \
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_OPENGL);

	if (win == NULL) {
		printf("error creating sdl_window: %s\n", SDL_GetError());
		return (1);
	}


	loopStopper();
	SDL_DestroyWindow(win);


	SDL_Quit();

	return (0);
}
