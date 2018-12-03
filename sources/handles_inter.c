/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_inter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 20:44:01 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:22:00 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void		set_selection_inter(t_selection *sel, t_ray ray, t_vec3 dir, \
	double t)
{
	sel->last_pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
	sel->view_axis = vec3_norm(vec3_sub(ray.origin, sel->last_pos));
	sel->motion_axis = dir;
	if ((sel->mode & SCOP_ROTATE) && sel->type > 0)
		dir = vec3_norm(vec3_cross(\
			dir, vec3_sub(sel->last_pos, sel->transform.position)));
	sel->proj_axis = vec3_norm(vec3_sub(\
		dir, vec3_scale(sel->view_axis, vec3_dot(dir, sel->view_axis))));
}

static void		translate_handles_inter(t_selection *sel, t_ray ray, double *t)
{
	t_obj_param		obj_p;
	unsigned int	i;

	i = 1;
	while (i < 4)
	{
		obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_DOWN, 0));
		obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i]), vec3_xyz(0, 4, 0), 1);
		obj_p.height = 4.0 * sel->transform.scale.x * sel->scale[0].x;
		obj_p.radius = 2.0 * sel->transform.scale.x * sel->scale[0].x;
		if (cone_inter(ray, obj_p, atan(0.5), t) && (sel->type = i) > -1)
			set_selection_inter(sel, ray, vec3_scale(obj_p.dir, -1), *t);
		obj_p.dir = vec3_scale(obj_p.dir, -1);
		obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i + 3]), (t_vec3)VEC3_ZERO, 1);
		obj_p.height = 8.0 * sel->transform.scale.x * sel->scale[i].y;
		obj_p.radius = LINE_RADIUS * sel->transform.scale.x;
		if (cylinder_inter(ray, obj_p, t) && (sel->type = i) > -1)
			set_selection_inter(sel, ray, obj_p.dir, *t);
		i++;
	}
}

static void		rotate_handles_inter(t_selection *sel, t_ray ray, double *t)
{
	t_obj_param obj_p;
	int			i;

	obj_p.pos = sel->transform.position;
	obj_p.radius = 1.0 * sel->transform.scale.x * sel->scale[4].x;
	if (sphere_inter(ray, obj_p, t))
	{
		sel->type = 0;
		set_selection_inter(sel, ray, (t_vec3)VEC3_ZERO, *t);
		obj_p.height = 0.125 * LINE_RADIUS * sel->transform.scale.x;
		i = 0;
		while (i++ < 3)
		{
			obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
				go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_FRONT, 0));
			if (circle_inter(ray, obj_p, t) && (sel->type = i) > -1)
				set_selection_inter(sel, ray, obj_p.dir, *t);
		}
	}
}

static void		scale_handles_inter(t_selection *sel, t_ray ray, double *t)
{
	t_obj_param		obj_p;
	t_vec3			up_vec;
	int				i;

	i = -1;
	while (++i < 4)
	{
		obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_UP, 0));
		up_vec = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_FRONT, 0));
		obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_ZERO, 1);
		obj_p.height = 1.0 * sel->transform.scale.x;
		obj_p.height *= (i) ? sel->scale[0].x : sel->scale[4].x;
		if (cube_inter(ray, obj_p, up_vec, t) && (sel->type = i) > -1)
			set_selection_inter(sel, ray, obj_p.dir, *t);
		obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
			go_trs(sel->transform), sel->rot[i + 3]), (t_vec3)VEC3_ZERO, 1);
		obj_p.height = 8.0 * sel->transform.scale.x * sel->scale[i].y;
		obj_p.radius = LINE_RADIUS * sel->transform.scale.x;
		if (sel->type != 0 && i > 0 && cylinder_inter(ray, obj_p, t) && \
				(sel->type = i) > 0)
			set_selection_inter(sel, ray, obj_p.dir, *t);
	}
}

unsigned int	handles_inter(t_env *env, int x, int y, int mouse_down)
{
	t_ray	ray;
	double	t;

	t = T_MAX;
	get_mouse_ray(env, &ray, x, y);
	env->selection.type = -1;
	set_selection_colors(&env->selection);
	if (env->selection.mode & SCOP_TRANSLATE)
		translate_handles_inter(&env->selection, ray, &t);
	else if (env->selection.mode & SCOP_ROTATE)
		rotate_handles_inter(&env->selection, ray, &t);
	else if (env->selection.mode & SCOP_SCALE)
		scale_handles_inter(&env->selection, ray, &t);
	if (env->selection.type == -1)
		return (0);
	else if (mouse_down)
	{
		vec4_v3(&env->selection.colors[env->selection.type], \
				vec3_norm((t_vec3)SCOP_MDOWN));
		env->selection.active = 1;
	}
	else
		vec4_v3(&env->selection.colors[env->selection.type], \
				vec3_norm((t_vec3)SCOP_MOVER));
	return (1);
}
