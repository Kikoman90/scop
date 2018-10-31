/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 11:15:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 14:06:55 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"


static void handle_window_events(t_env *env, SDL_Event *event)
{
    (void)env;
    (void)event;
    printf("WINDOW EVENT YAY\n");
}

static void handle_keyboard_events(t_env *env, SDL_Event *event)
{
    if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE)
        env->loop = 0;
    printf("KEYBOARD EVENT YAY\n");
}

static void handle_mouse_events(t_env *env, SDL_Event *event)
{
    (void)env;
    (void)event;
    printf("MOUSE EVENT YAY\n");
}

void        handle_events_and_input(t_env *env)
{
    SDL_Event   event;

    SDL_PollEvent(&event);
    if (event.type == SDL_WINDOWEVENT)
        handle_window_events(env, &event);
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        handle_keyboard_events(env, &event);
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ||\
        event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEWHEEL)
        handle_mouse_events(env, &event);
}

//if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
//	env->loop = 0;

//SDL_MaximizeWindow(sdl_window);
//SDL_GetWindowSize(sdl_window, &window_width, &window_height);