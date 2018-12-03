/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_manip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 17:06:07 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:24:34 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	sel_transform(t_selection *sel, t_vec3 delta, t_quaternion q)
{
	t_tr_node	*tmp;
	t_vec3		ratio;

	tmp = sel->list.head;
	while (tmp)
	{
		if (sel->mode & SCOP_TRANSLATE)
			tmp->transform->position = \
				vec3_add(tmp->transform->position, delta);
		else if (tmp->id != 1 && (sel->mode & SCOP_ROTATE))
			tmp->transform->rotation = quat_mult(tmp->transform->rotation, q);
		else if (tmp->id != 1)
		{
			ratio = vec3_norm(tmp->transform->scale);
			tmp->transform->scale.x += delta.x * fabsf(ratio.x);
			tmp->transform->scale.y += delta.y * fabsf(ratio.y);
			tmp->transform->scale.z += delta.z * fabsf(ratio.z);
		}
		tmp = tmp->next;
	}
}

static void	t_manip(t_selection *sel, t_ray ray, double t)
{
	float	delta;
	t_vec3	pos;
	t_vec3	sub;
	t_vec3	delta_v;

	if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
		return ;
	pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
	sub = vec3_sub(pos, sel->last_pos);
	delta = vec3_dot(sel->proj_axis, sub);
	delta_v = vec3_scale(sel->motion_axis, delta);
	sel_transform(sel, delta_v, quat());
	sel->last_pos = pos;
}

static void	r_manip(t_selection *sel, t_ray ray, double t)
{
	float			delta;
	t_vec3			pos;
	t_vec3			sub;
	t_quaternion	delta_q;

	if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
		return ;
	pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
	sub = vec3_sub(pos, sel->last_pos);
	delta = (360.0f * vec3_length(sub)) / \
		(2.0f * M_PI * sel->transform.scale.x * sel->scale[4].x);
	if (sel->type == 0)
		sel->motion_axis = vec3_norm(vec3_cross(sel->view_axis, sub));
	else
		delta *= vec3_dot(sel->proj_axis, vec3_norm(sub));
	delta_q = quat_tv(-delta, sel->motion_axis);
	sel_transform(sel, (t_vec3)VEC3_ZERO, delta_q);
	sel->last_pos = pos;
}

static void	s_manip(t_selection *sel, t_ray ray, t_vec3 motion, double t)
{
	float	delta;
	t_vec3	pos;
	t_vec3	sub;
	t_vec3	delta_v;

	if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
		return ;
	pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
	sub = vec3_sub(pos, sel->last_pos);
	delta = (sel->type) ? vec3_dot(sel->proj_axis, sub) : vec3_length(sub) * \
		vec3_dot(vec3_norm(vec3_xyz(0.5f, 0.5f, 0)), motion);
	sel->scale[1].y += (sel->type == 0 || sel->type == 1) ? delta / 8.0f : 0;
	sel->scale[2].y += (sel->type == 0 || sel->type == 2) ? delta / 8.0f : 0;
	sel->scale[3].y += (sel->type == 0 || sel->type == 3) ? delta / 8.0f : 0;
	sel->offset[1].x += (sel->type == 0 || sel->type == 1) ? delta : 0;
	sel->offset[2].y += (sel->type == 0 || sel->type == 2) ? delta : 0;
	sel->offset[3].z += (sel->type == 0 || sel->type == 3) ? delta : 0;
	delta_v = (sel->type) ? vec3_f(0) : vec3_f(delta);
	if (sel->type)
		*(&delta_v.x + (sel->type - 1)) = delta;
	sel_transform(sel, delta_v, quat());
	sel->last_pos = pos;
}

void		handles_manip(t_env *env, t_vec2 motion, t_vec2 pos)
{
	t_ray	ray;
	double	t;

	t = T_MAX;
	get_mouse_ray(env, &ray, pos.x, pos.y);
	if (env->selection.mode & SCOP_TRANSLATE)
		t_manip(&env->selection, ray, t);
	else if (env->selection.mode & SCOP_ROTATE)
		r_manip(&env->selection, ray, t);
	else if (env->selection.mode & SCOP_SCALE)
		s_manip(&env->selection, ray, \
				vec3_norm(vec3_xyz(motion.x, -motion.y, 0)), t);
	set_selection_mode(&env->selection, env->selection.mode, 0);
}
