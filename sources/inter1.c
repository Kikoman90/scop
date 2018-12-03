/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:03 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:27:40 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"

int		solve_quadratic(double c[3], double t[2], double t_max)
{
	double	disc;

	disc = c[1] * c[1] - 4.0 * c[0] * c[2];
	if (disc < 0)
		return (0);
	if (disc == 0)
	{
		t[0] = -c[1] / (2.0 * c[0]);
		return ((t[0] >= T_MIN && t[0] < t_max));
	}
	t[0] = (-c[1] - sqrt(disc)) / (2.0 * c[0]);
	t[1] = (-c[1] + sqrt(disc)) / (2.0 * c[0]);
	if (t[0] >= T_MIN && t[0] < t_max)
	{
		if (t[0] > t[1] && t[1] >= T_MIN && t[1] < t_max)
		{
			t[0] = t[1];
			return (2);
		}
		return (1);
	}
	t[0] = t[1];
	return ((t[0] >= T_MIN && t[0] < t_max));
}

int		circle_inter(t_ray ray, t_obj_param p, double *t)
{
	t_vec3	hit;
	double	dist;

	hit = vec3_add(ray.origin, vec3_scale(ray.dir, *t));
	dist = vec3_dot(p.dir, vec3_sub(hit, p.pos));
	if (fabs(dist) <= p.height)
	{
		hit = vec3_add(hit, vec3_scale(p.dir, dist));
		*t = vec3_length(vec3_sub(hit, ray.origin));
		return (1);
	}
	return (0);
}

int		sphere_inter(t_ray ray, t_obj_param p, double *t)
{
	t_inter	inter;

	inter.v[0] = vec3_sub(ray.origin, p.pos);
	inter.c[0] = vec3_dot(ray.dir, ray.dir);
	inter.c[1] = 2.0 * vec3_dot(ray.dir, inter.v[0]);
	inter.c[2] = vec3_dot(inter.v[0], inter.v[0]) - p.radius * p.radius;
	if (!(inter.ret = solve_quadratic(inter.c, inter.t, *t)))
		return (0);
	*t = inter.t[0];
	return (1);
}

int		dbl_plane_inter(t_inter *inter, t_ray ray, t_obj_param p, t_vec3 dir)
{
	double	swap;
	t_vec3	side_pos;
	t_vec3	side_dir;

	side_pos = vec3_add(p.pos, vec3_scale(dir, -p.height));
	side_dir = vec3_scale(dir, -1);
	inter->t[0] = vec3_dot(side_dir, vec3_sub(side_pos, ray.origin)) /\
		vec3_dot(side_dir, ray.dir);
	side_pos = vec3_add(p.pos, vec3_scale(dir, p.height));
	side_dir = dir;
	inter->t[1] = vec3_dot(side_dir, vec3_sub(side_pos, ray.origin)) /\
		vec3_dot(side_dir, ray.dir);
	if (inter->t[0] > inter->t[1])
	{
		swap = inter->t[0];
		inter->t[0] = inter->t[1];
		inter->t[1] = swap;
	}
	if (inter->t[0] > inter->t[2])
		inter->t[2] = inter->t[0];
	if (inter->t[1] < inter->t[3])
		inter->t[3] = inter->t[1];
	if (inter->t[3] < inter->t[2])
		return (0);
	return (1);
}

int		cube_inter(t_ray ray, t_obj_param p, t_vec3 up_vec, double *t)
{
	t_inter		inter;

	inter.t[2] = T_MIN;
	inter.t[3] = T_MAX;
	inter.v[0] = vec3_norm(vec3_cross(p.dir, up_vec));
	if (!dbl_plane_inter(&inter, ray, p, p.dir))
		return (0);
	if (!dbl_plane_inter(&inter, ray, p, up_vec))
		return (0);
	if (!dbl_plane_inter(&inter, ray, p, inter.v[0]) || inter.t[2] >= *t)
		return (0);
	*t = inter.t[2];
	return (1);
}
