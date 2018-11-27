/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 18:24:25 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 17:34:19 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTION_H
# define INTERSECTION_H

# include "../libs/libmath/includes/libmath.h"

# define LINE_RADIUS 1.0

# define EQN_EPS			1e-9
# define ISZERO(x)			((x) > -EQN_EPS && (x) < EQN_EPS)

# define CBRT_R(x)			((x) < 0.0 ? -pow((double)-(x), 1.0/3.0) : 0.0))
# define CBRT(x)			((x) > 0.0 ? pow((double)(x), 1.0/3.0) : CBRT_R(x)

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
    double      c[5];
    double      t[4];
    int         roots;
}               t_inter;

typedef struct  s_obj_param
{
    t_vec3      pos;
    t_vec3      dir;
    double      height;
    double      radius;
}               t_obj_param;

typedef struct  s_cubic
{
    double      sub;
    double      a;
    double      b;
    double      c;
    double      d;
    double      sq_a;
    double      p;
    double      q;
    double      cb_p;
    int			i;
    int         roots;
}               t_cubic;

typedef struct  s_quartic
{
    double      coeffs[4];
    double      z;
    double      u;
    double      v;
    double      sub;
    double      a;
    double      b;
    double      c;
    double      d;
    double      sq_a;
    double      p;
    double      q;
    double      r;
    int         i;
    int         roots;
}               t_quartic;

/*
** inter1.c     => 5 functions
*/
int             solve_quadratic(double c[3], double t[2]);
unsigned int    sphere_inter(t_ray ray, t_obj_param p, double *t);
unsigned int    cube_inter(t_ray ray, t_obj_param p, t_vec3 up_vec, double *t);
unsigned int    torus_inter(t_ray ray, t_obj_param obj_p, double *t);

/*
** inter.2c     => 4 functions
*/
unsigned int    cone_inter(t_ray ray, t_obj_param p, double angle, double *t);
unsigned int    cylinder_inter(t_ray ray, t_obj_param p, double *t);


/*
** solve_quartic.c  => 5 functions
*/
int             solve_quartic(double c[5], double t[4]);

/*
** solve_cubic.c    => 5 functions
*/
int             solve_cubic(double c[4], double t[3]);

#endif
