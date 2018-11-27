/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 15:32:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 15:33:29 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"

static unsigned int caps_inter(t_inter inter, t_ray ray, double *t, \
    int cylinder)
{
    unsigned int    ret;
    t_vec3          dist;

    ret = inter.roots;
    if (cylinder && (inter.t[0] = vec3_dot(inter.v[0], \
        vec3_sub(inter.v[1], ray.origin)) / vec3_dot(inter.v[0], \
        ray.dir)) >= T_MIN)
    {
        dist = vec3_add(ray.origin, vec3_scale(ray.dir, inter.t[0]));
        dist = vec3_sub(dist, inter.v[1]);
        if (inter.t[0] < *t && vec3_dot(dist, dist) <= inter.sqrad && (ret = 1))
            *t = inter.t[0];
    }
    if ((inter.t[0] = vec3_dot(inter.v[0], vec3_sub(inter.v[2], ray.origin)) /\
        vec3_dot(inter.v[0], ray.dir)) >= T_MIN)
    {
        dist = vec3_add(ray.origin, vec3_scale(ray.dir, inter.t[0]));
        dist = vec3_sub(dist, inter.v[2]);
        if (inter.t[0] < *t && vec3_dot(dist, dist) <= inter.sqrad && (ret = 1))
            *t = inter.t[0];
    }
    return (ret);
}

static void         solve_primitive_lim(t_inter *inter, t_ray ray, double *t)
{
    int     i;
    t_vec3  pos;

    i = inter->roots;
    while (i--)
    {
        pos = vec3_add(ray.origin, vec3_scale(ray.dir, inter->t[i]));
        if (vec3_dot(inter->v[0], vec3_sub(pos, inter->v[1])) > 0 && \
            vec3_dot(inter->v[0], vec3_sub(pos, inter->v[2])) < 0 && \
            inter->t[i] >= T_MIN && inter->t[i] < *t)
            *t = inter->t[i];
        else
            inter->roots -= 1;
    }
}

unsigned int        cone_inter(t_ray ray, t_obj_param p, double angle, \
    double *t)
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
    if ((inter.roots = solve_quadratic(inter.c, inter.t)))
        solve_primitive_lim(&inter, ray, t);
    inter.sqrad = p.radius * p.radius;
    return (caps_inter(inter, ray, t, 0));
}

unsigned int        cylinder_inter(t_ray ray, t_obj_param p, double *t)
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
    if ((inter.roots = solve_quadratic(inter.c, inter.t)))
        solve_primitive_lim(&inter, ray, t);
    inter.sqrad = p.radius * p.radius;
    return (caps_inter(inter, ray, t, 1));
}
