/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 18:24:25 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 13:55:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTION_H
# define INTERSECTION_H

# include "../libs/libmath/includes/libmath.h"

# define LINE_RADIUS 1.0

# define T_MIN 0.001
# define T_MAX 100.0

typedef struct  s_ray
{
    t_vec3      origin;
    t_vec3      dir;
}               t_ray;

typedef struct  s_inter
{
    t_vec3      v[3];
    double      sqrad;
    double      c[3];
    double      t[4];
    int         ret;
}               t_inter;

typedef struct  s_obj_param
{
    t_vec3      pos;
    t_vec3      dir;
    double      height;
    double      radius;
}               t_obj_param;

/*
** inter1.c     => 5 functions
*/
int             solve_quadratic(double c[3], double t[2], double t_max);
int             sphere_inter(t_ray ray, t_obj_param p, double *t);
int             cube_inter(t_ray ray, t_obj_param p, t_vec3 up_vec, double *t);
int             circle_inter(t_ray ray, t_obj_param p, double t);

/*
** inter2.c     => 4 functions
*/
int             cone_inter(t_ray ray, t_obj_param p, double angle, double *t);
int             cylinder_inter(t_ray ray, t_obj_param p, double *t);

#endif
