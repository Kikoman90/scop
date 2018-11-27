/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 11:15:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 20:37:24 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	handle_window_events(t_env *env, SDL_Event *event)
{
	int	w;
	int	h;

	if (event->window.event == SDL_WINDOWEVENT_CLOSE)
		env->loop = 0;
	else if (event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_GetWindowSize(env->win_env.window, &w, &h);
		glViewport(0, 0, w, h);
		generate_framebuffers(&env->buffers, w, h);
		env->win_env.win_w = w;
		env->win_env.win_h = h;
		env->matrices.update_mat[2] = 1;
	}
}

static void	handle_mouse_events(t_env *env, SDL_Event *event, \
	const Uint8 *kstate)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		if (event->motion.state & SDL_BUTTON_RMASK)
		{
			if (kstate[SDL_SCANCODE_LSHIFT] || kstate[SDL_SCANCODE_RSHIFT])
				update_view(env, vec3_xyz(-event->motion.xrel, \
					event->motion.yrel, 0), SCOP_TRANSLATE);
			else if (kstate[SDL_SCANCODE_LCTRL] || kstate[SDL_SCANCODE_RCTRL])		
				update_view(env, vec3_xyz(0, 0, event->motion.yrel), \
					SCOP_SCALE);
			else
				update_view(env, vec3_xyz(\
					-event->motion.xrel, -event->motion.yrel, 0), SCOP_ROTATE);
		}
		else if (event->motion.state & SDL_BUTTON_LMASK)
		{
			;//if (env->selection.active)
			//	;//handle manip;
		}
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN && \
		event->button.button == SDL_BUTTON_LEFT)
	{
		handles_inter(env);
		if (!env->selection.active)
			picking_check(env, event->button.x, event->button.y, kstate[SDL_SCANCODE_LSHIFT]);
	}
	else if (event->type == SDL_MOUSEBUTTONUP && \
		event->button.button == SDL_BUTTON_LEFT)
	{
		env->selection.active = 0;
		set_selection_mode(&env->selection, env->selection.mode); //vec3_norm(vec3_v4(env->matrices.v.v[2])));
	}
	else if (event->type == SDL_MOUSEWHEEL)
		update_view(env, vec3_xyz(0, 0, -event->wheel.y), SCOP_SCALE);
}
// LASTLY, SETUP AN ORTHO PROJECTION FOR THE HANDLES

static void	handle_smooth_keys(t_env *env, const Uint8 *keyboard_state)
{
	if (keyboard_state[SDL_SCANCODE_RIGHT])
		update_view(env, vec3_xyz(8, 0, 0), SCOP_TRANSLATE);
	else if (keyboard_state[SDL_SCANCODE_LEFT])
		update_view(env, vec3_xyz(-8, 0, 0), SCOP_TRANSLATE);
	else if (keyboard_state[SDL_SCANCODE_UP])
		update_view(env, vec3_xyz(0, 0, -8), SCOP_TRANSLATE);
	else if (keyboard_state[SDL_SCANCODE_DOWN])
		update_view(env, vec3_xyz(0, 0, 8), SCOP_TRANSLATE);
	else if (keyboard_state[SDL_SCANCODE_KP_MINUS])
		update_view(env, vec3_xyz(0, -8, 0), SCOP_TRANSLATE);
	else if (keyboard_state[SDL_SCANCODE_KP_PLUS])
		update_view(env, vec3_xyz(0, 8, 0), SCOP_TRANSLATE);
}

static void	handle_key_events(t_env *env, Uint8 scancode)
{
	if (scancode == SDL_SCANCODE_ESCAPE)
		env->loop = 0;
	else if (scancode == SDL_SCANCODE_Z)
		env->input.auto_rotate = (env->input.auto_rotate) ? 0 : 1;
	else if (scancode == SDL_SCANCODE_X)
	{
		env->selection.localspace = (env->selection.localspace) ? 0 : 1;
		set_selection_transform(&env->selection);
	}
	else if (scancode == SDL_SCANCODE_C)
		env->input.face_rgb = (env->input.face_rgb) ? 0 : 1;
	else if (scancode == SDL_SCANCODE_S)
		env->input.cur_sky += (env->input.cur_sky == 3) ? -3 : 1;
	else if (scancode == SDL_SCANCODE_T)
		env->input.cur_tex += (env->input.cur_tex == 6) ? -6 : 1;
	else if (scancode == SDL_SCANCODE_W)
		set_selection_mode(&env->selection, SCOP_TRANSLATE);
	else if (scancode == SDL_SCANCODE_E)
		set_selection_mode(&env->selection, SCOP_ROTATE);		
	else if (scancode == SDL_SCANCODE_R)
		set_selection_mode(&env->selection, SCOP_SCALE);
	else if (scancode == SDL_SCANCODE_SPACE)
	{
		if (env->input.fade)
			env->input.fade_coef = (env->input.fade_coef >= 0) ? -1.05f : 1.05f;
		else
			env->input.fade_coef = 1.05f;
	}
}

void		handle_events_and_input(t_env *env)
{
	SDL_Event	event;
	const Uint8	*keyboard_state;

	keyboard_state = SDL_GetKeyboardState(NULL);
	handle_smooth_keys(env, keyboard_state);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
			handle_window_events(env, &event);
		else if (event.type == SDL_MOUSEBUTTONDOWN ||\
			event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION ||\
			event.type == SDL_MOUSEWHEEL)
			handle_mouse_events(env, &event, keyboard_state);
		else if (event.type == SDL_KEYDOWN)
			handle_key_events(env, event.key.keysym.scancode);
		else if (event.type == SDL_QUIT)
			env->loop = 0;
	}
	if (env->selection.list.count > 0 && !env->selection.active)
		handles_inter(env);
}
