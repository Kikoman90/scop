/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 15:32:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 14:25:36 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"

static int  caps_inter(t_inter inter, t_ray ray, double *t, int cylinder)
{
    double          d;
    t_vec3          dist;

    if (cylinder && (d = vec3_dot(inter.v[0], vec3_sub(inter.v[1], ray.origin))\
        / vec3_dot(inter.v[0], ray.dir)) >= T_MIN && d < *t)
    {
        dist = \
            vec3_sub(vec3_add(ray.origin, vec3_scale(ray.dir, d)), inter.v[1]);
        if (vec3_dot(dist, dist) <= inter.sqrad && (inter.ret = 1))
            *t = d;
    }
    if ((d = vec3_dot(inter.v[0], vec3_sub(inter.v[2], ray.origin)) /\
        vec3_dot(inter.v[0], ray.dir)) >= T_MIN && d < *t)
    {
        dist = \
            vec3_sub(vec3_add(ray.origin, vec3_scale(ray.dir, d)), inter.v[2]);
        if (vec3_dot(dist, dist) <= inter.sqrad && (inter.ret = 1))
            *t = d;
    }
    return (inter.ret);
}

static void solve_primitive_lim(t_inter *inter, t_ray ray, double *t)
{
    t_vec3  pos;

    pos = vec3_add(ray.origin, vec3_scale(ray.dir, inter->t[0]));
    if (vec3_dot(inter->v[0], vec3_sub(pos, inter->v[1])) > 0 && \
        vec3_dot(inter->v[0], vec3_sub(pos, inter->v[2])) < 0)
        *t = inter->t[0];
    else
        inter->ret = 0;
}

int         cone_inter(t_ray ray, t_obj_param p, double angle, double *t)
{
    t_inter inter;
        
    inter.v[0] = vec3_sub(ray.origin, p.pos);
    inter.v[1] = vec3_sub(ray.dir, vec3_scale(p.dir, vec3_dot(ray.dir, p.dir)));
    inter.v[2] = vec3_sub(inter.v[0], \
        vec3_scale(p.dir, vec3_dot(inter.v[0], p.dir)));
    inter.c[0] = pow(cos(angle), 2) * vec3_dot(inter.v[1], inter.v[1]) - \
        pow(sin(angle), 2) * pow(vec3_dot(ray.dir, p.dir), 2);
    inter.c[1] = 2.0 * pow(cos(angle), 2) * vec3_dot(inter.v[1], inter.v[2]) - \
        2.0 * pow(sin(angle), 2) * \
            vec3_dot(ray.dir, p.dir) * vec3_dot(inter.v[0], p.dir);
    inter.c[2] = pow(cos(angle), 2) * vec3_dot(inter.v[2], inter.v[2]) - \
        pow(sin(angle), 2) * pow(vec3_dot(inter.v[0], p.dir), 2);
    inter.v[0] = p.dir;
    inter.v[1] = p.pos;
    inter.v[2] = vec3_add(p.pos, vec3_scale(p.dir, p.height));
    inter.sqrad = p.radius * p.radius;
    if ((inter.ret = solve_quadratic(inter.c, inter.t, *t)))
        solve_primitive_lim(&inter, ray, t);
    return (caps_inter(inter, ray, t, 0));
}

int         cylinder_inter(t_ray ray, t_obj_param p, double *t)
{
    t_inter inter;

    inter.v[0] = vec3_sub(ray.origin, p.pos);
    inter.v[1] = vec3_sub(ray.dir, vec3_scale(p.dir, vec3_dot(ray.dir, p.dir)));
    inter.v[2] = vec3_sub(inter.v[0], \
        vec3_scale(p.dir, vec3_dot(inter.v[0], p.dir)));
    inter.c[0] = vec3_dot(inter.v[1], inter.v[1]);
    inter.c[1] = 2.0 * vec3_dot(inter.v[1], inter.v[2]);
    inter.c[2] = vec3_dot(inter.v[2], inter.v[2]) - p.radius * p.radius;
    inter.v[0] = p.dir;
    inter.v[1] = p.pos;
    inter.v[2] = vec3_add(p.pos, vec3_scale(p.dir, p.height));
    inter.sqrad = p.radius * p.radius;
    if ((inter.ret = solve_quadratic(inter.c, inter.t, *t)))
        solve_primitive_lim(&inter, ray, t);
    return (caps_inter(inter, ray, t, 1));
}
