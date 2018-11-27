/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:03 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 20:32:21 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"
#include <stdio.h>//

int             solve_quadratic(double c[3], double t[2])
{
    double  disc;

    disc = c[1] * c[1] - 4.0 * c[0] * c[2];
    if (disc < 0)
        return (0);
    if (disc == 0)
    {
        t[0] = -c[1] / (2.0 * c[0]);
        return (1);
    }
    t[0] = (-c[1] - sqrt(disc)) / (2.0 * c[0]);
    t[1] = (-c[1] + sqrt(disc)) / (2.0 * c[0]);
    return (2);
}

unsigned int    solve_inter(t_inter inter, double *t)
{
    int i;
    int success;

    i = 0;
    success = 0;
    while (i < inter.roots)
    {
        if (inter.t[i] >= T_MIN && inter.t[i] < *t)
        {
            success = 1;
            *t = inter.t[i];
        }
        i++;
    }
    return (success);
}

unsigned int    sphere_inter(t_ray ray, t_obj_param p, double *t)
{
    t_inter inter;

    inter.v[0] = vec3_sub(ray.origin, p.pos);
    inter.c[0] = vec3_dot(ray.dir, ray.dir);
    inter.c[1] = 2.0 * vec3_dot(ray.dir, inter.v[0]);
    inter.c[2] = vec3_dot(inter.v[0], inter.v[0]) - p.radius * p.radius;
    if (!(inter.roots = solve_quadratic(inter.c, inter.t)))
        return (0);
    return (solve_inter(inter, t));
}

unsigned int    dbl_plane_inter(t_inter *inter, t_ray ray, t_obj_param p, \
    t_vec3 dir)
{
    double  swap;
    t_vec3  side_pos;
    t_vec3  side_dir;

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

unsigned int    cube_inter(t_ray ray, t_obj_param p, t_vec3 up_vec, double *t)
{
    t_inter     inter;

    inter.t[2] = T_MIN;
    inter.t[3] = T_MAX;
    inter.v[0] = vec3_norm(vec3_cross(p.dir, up_vec));
    if (!dbl_plane_inter(&inter, ray, p, p.dir))
        return (0);
    if (!dbl_plane_inter(&inter, ray, p, up_vec))
        return (0);
    if (!dbl_plane_inter(&inter, ray, p, inter.v[0]))
        return (0);
    if (inter.t[2] >= *t)
        return (0);
    *t = inter.t[2];
    return (1);
}

unsigned int    torus_inter(t_ray ray, t_obj_param p, double *t)
{
    t_inter inter;
    double  u;
    double  v;

    inter.v[0] = vec3_sub(ray.origin, p.pos);
    u = vec3_dot(p.dir, inter.v[0]);
    v = vec3_dot(p.dir, ray.dir);
    inter.sqrad = p.radius * p.radius;
    inter.t[0] = 1.0 - v * v;
    inter.t[1] = 2.0 * (vec3_dot(inter.v[0], ray.dir) - u * v);
    inter.t[2] = vec3_dot(inter.v[0], inter.v[0]) - u * u;
    inter.t[3] = vec3_dot(inter.v[0], inter.v[0]) + inter.sqrad - \
        p.height * p.height;
    inter.c[4] = 1.0;
    inter.c[3] = 4.0 * vec3_dot(inter.v[0], ray.dir);
    inter.c[2] = 2.0 * inter.t[3] + 0.25 * inter.c[3] * inter.c[3] \
        - 4.0 * inter.sqrad * inter.t[0];
    inter.c[1] = inter.c[3] * inter.t[3] - 4.0 * inter.sqrad * inter.t[1];
    inter.c[0] = inter.t[3] * inter.t[3] - 4.0 * inter.sqrad * inter.t[2];
    //
    inter.t[0] = -1;
    inter.t[1] = -1;
    inter.t[2] = -1;
    inter.t[3] = -1;
    //
    if (!(inter.roots = solve_quartic(inter.c, inter.t)))
        return (0);
    return (solve_inter(inter, t));
}
