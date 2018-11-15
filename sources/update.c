/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:43:49 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 12:15:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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
		env->matrices.update_mat[1] = 0;//
	}
	if (env->matrices.update_mat[2] == 1 || update == 2)
	{
		env->matrices.p = compute_projection(env->camera.fov, \
			(float)env->win_env.win_w / env->win_env.win_h, \
			env->camera.znear, env->camera.zfar);
		env->matrices.update_mat[2] = 0;
	}
		
	/*if (env->matrices.update_mat[1] == 1 || env->matrices.update_mat[2] == 1 \
		|| update == 1 || update == 2)
	{
		env->matrices.vp = \
			mat4x4_mult(env->matrices.projection, env->matrices.view);
		env->matrices.update_mat[1] = 0;
		env->matrices.update_mat[2] = 0;
	}*/
}

/*void	update_view(t_camera *camera, SDL_MouseMotionEvent motion, \
	double delta_time, float speed)
{
	;
	//camera->transform.rotation = quat_norm(camera->transform.rotation);
}*/