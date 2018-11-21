/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 18:24:25 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 19:28:46 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTION_H
# define INTERSECTION_H

# include "../libs/libmath/includes/libmath.h"

# define LINE_RADIUS 0.2

typedef struct  s_ray
{
    t_vec3      origin;
    t_vec3      dir;
}               t_ray;

typedef struct  s_inter
{
    t_vec3      dist;
    double      a;
    double      b;
    double      c;
    double      discriminant;
    double      t0;
    double      t1;
}               t_inter;

typedef struct  s_obj_param
{
    t_vec3      pos;
    t_vec3      dir;
    double      height;
    double      radius;
}               t_obj_param;

/*
** primitive_inter.c => 5 functions
*/
unsigned int    sphere_inter(t_ray ray, t_obj_param obj_p, double *t);
//unsigned int    torus_inter(t_ray ray, t_obj_param obj_p, double *t);

#endif
