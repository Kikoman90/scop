/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 11:15:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/08 20:30:07 by fsidler          ###   ########.fr       */
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

static void	handle_keyboard_events(t_env *env, const Uint8 *keyboard_state, \
		double delta_time)
{
	t_vec3	axes[3];

	(void)delta_time;
	get_matrix_axes(&axes, mat4x4_transpose(quat_to_mat4x4(env->camera.transform.rotation)));
	if (keyboard_state[SDL_SCANCODE_ESCAPE])
		env->loop = 0;
	else if (keyboard_state[SDL_SCANCODE_RIGHT])
	{
		env->camera.transform.position = vec3_add(\
			env->camera.transform.position, \
			vec3_scale(axes[0], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);
	}
	else if (keyboard_state[SDL_SCANCODE_LEFT])
	{
		env->camera.transform.position = vec3_sub(\
			env->camera.transform.position, \
			vec3_scale(axes[0], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);		
	}
	else if (keyboard_state[SDL_SCANCODE_UP])
	{
		env->camera.transform.position = vec3_sub(\
			env->camera.transform.position, \
			vec3_scale(axes[2], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);
	}
	else if (keyboard_state[SDL_SCANCODE_DOWN])
	{
		env->camera.transform.position = vec3_add(\
			env->camera.transform.position, \
			vec3_scale(axes[2], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);
	}
	else if (keyboard_state[SDL_SCANCODE_KP_MINUS])
	{
		env->camera.transform.position = vec3_sub(\
			env->camera.transform.position, \
			vec3_scale(axes[1], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);
	}
	else if (keyboard_state[SDL_SCANCODE_KP_PLUS])
	{
		env->camera.transform.position = vec3_add(\
			env->camera.transform.position, \
			vec3_scale(axes[1], delta_time * env->input.pan_speed));
		//update_matrices(env, 1);		
	}
}

static void	handle_mouse_events(t_env *env, SDL_Event *event, \
	double delta_time, int lsp)
{
	t_vec3	axes[3];

	get_matrix_axes(&axes, mat4x4_transpose(quat_to_mat4x4(env->camera.transform.rotation)));
	if (event->type == SDL_MOUSEMOTION)// && event->motion.state & SDL_BUTTON_RMASK)
	{
		if (event->motion.state & SDL_BUTTON_RMASK)
		{
			env->camera.transform.position = vec3_add(\
				env->camera.transform.position, \
				vec3_scale(axes[2], event->motion.yrel * delta_time * env->input.zoom_speed));
		}
		else if (lsp)
		{
			env->camera.transform.position = vec3_add(\
				env->camera.transform.position, \
				vec3_scale(axes[0], -event->motion.xrel * delta_time));
			env->camera.transform.position = vec3_add(\
				env->camera.transform.position, \
				vec3_scale(axes[1], event->motion.yrel * delta_time));
		}
		else
		{
			env->camera.transform.rotation = quat_mult(\
				env->camera.transform.rotation, quat_tv(-event->motion.yrel \
				* env->input.orbit_speed * delta_time, (t_vec3)VEC3_RIGHT));
			env->camera.transform.rotation = quat_mult(quat_tv(\
				-event->motion.xrel * env->input.orbit_speed * delta_time, \
				(t_vec3)VEC3_UP), env->camera.transform.rotation);
		}
		//update_matrices(env, 1);		
		//update_view(&env->camera, event->motion, delta_time, 11.2f);
	}
	else if (event->type == SDL_MOUSEWHEEL)
	{
		env->camera.transform.position = vec3_add(\
			env->camera.transform.position, \
			vec3_scale(axes[2], -event->wheel.y * env->input.zoom_speed \
			* delta_time));
		//update_matrices(env, 1);		
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{

	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{

	}
}

void		handle_events_and_input(t_env *env, double delta_time)
{
	SDL_Event	event;
	const Uint8	*keyboard_state;
	int			lshift_pressed;

	lshift_pressed = 0;
	while (SDL_PollEvent(&event))
	{
		/*else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
		{
			lshift_pressed = 1;
			printf("LSHIFT\n");
		}*/
		keyboard_state = SDL_GetKeyboardState(NULL);
		if (keyboard_state[SDL_SCANCODE_LSHIFT] || keyboard_state[SDL_SCANCODE_RSHIFT])
			lshift_pressed = 1;
		if (keyboard_state[SDL_SCANCODE_DELETE])
			SDL_SetWindowGrab(env->win_env.window, SDL_FALSE);
		if (event.type == SDL_WINDOWEVENT)
			handle_window_events(env, &event);
		else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION \
			|| event.type == SDL_MOUSEWHEEL)
			handle_mouse_events(env, &event, delta_time, lshift_pressed);
		//	handle_keyboard_events(env, keyboard_state, delta_time);
	}
	keyboard_state = SDL_GetKeyboardState(NULL);
	handle_keyboard_events(env, keyboard_state, delta_time);
	update_matrices(env, 1);
}
