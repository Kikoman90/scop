/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 11:15:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/19 20:54:33 by fsidler          ###   ########.fr       */
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

void		update_view(t_env *env, t_vec3 d, t_handlemode mode)
{
	t_transform	tr;
	t_vec3		axes[3];

	tr = env->camera.transform;
	get_matrix_axes(&axes, mat4x4_transpose(quat_to_mat4x4(tr.rotation)));
	if (mode & SCOP_TRANSLATE)
	{
		d = vec3_scale(d, env->input.pan_speed * env->delta_time);
		tr.position = vec3_add(tr.position, vec3_scale(axes[0], d.x));
		tr.position = vec3_add(tr.position, vec3_scale(axes[1], d.y));
		tr.position = vec3_add(tr.position, vec3_scale(axes[2], d.z));
	}
	else if (mode & SCOP_SCALE)
		tr.position = vec3_add(tr.position, vec3_scale(\
			axes[2], d.z * env->input.zoom_speed * env->delta_time));
	else if (mode & SCOP_ROTATE)
	{
		d = vec3_scale(d, env->input.rot_speed * env->delta_time);
		tr.rotation = quat_mult(tr.rotation, quat_tv(d.y, (t_vec3)VEC3_RIGHT));
		tr.rotation = quat_mult(quat_tv(d.x, (t_vec3)VEC3_UP), tr.rotation);
	}
	env->camera.transform = tr;
	env->matrices.update_mat[1] = 1;
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
			// if a handle is selected 
			//handle manip
		}
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			// first -> handle intersections. if no intersection -> object selection
			//
			picking_check(env, event->button.x, event->button.y, kstate[SDL_SCANCODE_LSHIFT]);
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
			env->selection.active = 0;
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
		env->selection.localspace = (env->selection.localspace) ? 0 : 1;
	else if (scancode == SDL_SCANCODE_C)
		env->input.face_rgb = (env->input.face_rgb) ? 0 : 1;
	else if (scancode == SDL_SCANCODE_S)
		env->input.cur_sky += (env->input.cur_sky == 3) ? -3 : 1;
	else if (scancode == SDL_SCANCODE_T)
		env->input.cur_tex += (env->input.cur_tex == 5) ? -5 : 1;
	else if (scancode == SDL_SCANCODE_W)
		env->selection.mode = SCOP_TRANSLATE;
	else if (scancode == SDL_SCANCODE_E)
		env->selection.mode = SCOP_ROTATE;
	else if (scancode == SDL_SCANCODE_R)
		env->selection.mode = SCOP_SCALE;
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
}
