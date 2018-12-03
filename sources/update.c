/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:43:49 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 19:07:51 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	handle_window_events(t_env *env, SDL_Event *event)
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

void	rotate_gameobjects(t_go_node *list, double delta)
{
	t_quaternion	z_rot;
	t_go_node		*tmp;

	z_rot = quat_tv(22.5f * delta, (t_vec3)VEC3_UP);
	tmp = list;
	while (tmp)
	{
		tmp->go->transform.rotation = \
			quat_mult(tmp->go->transform.rotation, z_rot);
		tmp = tmp->next;
	}
}

void	update_view(t_env *env, t_vec3 d, t_handlemode mode)
{
	t_transform	tr;
	t_vec3		axes[3];

	tr = env->camera.transform;
	get_matrix_axes(axes, mat4x4_transpose(quat_to_mat4x4(tr.rotation)));
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

void	update_matrices(t_env *env, int update)
{
	if (env->matrices.update_mat[0] == 1 || update == 0)
	{
		if (env->matrices.model)
			free(env->matrices.model);
		if (env->gameobjects.count == 0)
			env->matrices.update_mat[0] = 0;
		else if (!(env->matrices.model = (t_mat4x4*)malloc(sizeof(t_mat4x4) * \
			env->gameobjects.count)))
			log_error(MALLOC_ERROR);
		else
			env->matrices.update_mat[0] = 0;
	}
	if (env->matrices.update_mat[1] == 1 || update == 1)
	{
		env->matrices.v = compute_view(env->camera.transform);
		env->matrices.update_mat[1] = 0;
	}
	if (env->matrices.update_mat[2] == 1 || update == 2)
	{
		env->matrices.p = compute_perspective(env->camera.fov, \
			(float)env->win_env.win_w / env->win_env.win_h, \
			env->camera.znear, env->camera.zfar);
		env->matrices.update_mat[2] = 0;
	}
}
