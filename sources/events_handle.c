/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 11:15:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/05 18:14:30 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		resize_window(t_env *env)
{
	int	w;
	int	h;

	SDL_GetWindowSize(env->win_env.window, &w, &h);
	glViewport(0, 0, w, h);
	generate_framebuffers(&env->buffers, w, h);
	env->win_env.win_w = w;
	env->win_env.win_h = h;
	env->matrices.update_mat[2] = 1;
}

static void	handle_window_events(t_env *env, SDL_Event *event)
{
	if (event->window.event == SDL_WINDOWEVENT_CLOSE)
		env->loop = 0;
	else if (event->window.event == SDL_WINDOWEVENT_RESIZED)
		resize_window(env);
}

static void	handle_keyboard_events(t_env *env, SDL_Event *event)
{
	if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE)
		env->loop = 0;
}

static void	handle_mouse_events(t_env *env, SDL_Event *event, double delta_time, float speed)
{
	//SDL_SetRelativeMouseMode(SDL_TRUE); // -> setup.c
	if (event->type == SDL_MOUSEMOTION && event->motion.state & SDL_BUTTON_RMASK)
	{
		env->camera.transform.rotation = quat_mult(env->camera.transform.rotation, \
			quat_tv(-event->motion.yrel * speed * delta_time, (t_vec3)VEC3_RIGHT));
		env->camera.transform.rotation = quat_mult(quat_tv(-event->motion.xrel \
			* speed * delta_time, (t_vec3)VEC3_UP), env->camera.transform.rotation);
		env->matrices.update_mat[1] = 1;
	}
}

void		handle_events_and_input(t_env *env, double delta_time)
{
	SDL_Event	event;

	SDL_PollEvent(&event);
	if (event.type == SDL_WINDOWEVENT)
		handle_window_events(env, &event);
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		handle_keyboard_events(env, &event);
	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ||\
		event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEWHEEL)
		handle_mouse_events(env, &event, delta_time, 11.0f);
}
