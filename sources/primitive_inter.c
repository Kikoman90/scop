/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitive_inter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:03 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 19:35:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"
#include <stdio.h>//

unsigned int    sphere_inter(t_ray ray, t_obj_param obj_p, double *t)
{
    t_inter inter;

    inter.dist = vec3_sub(ray.origin, obj_p.pos);
    inter.a = vec3_dot(ray.dir, ray.dir);
    inter.b = 2.0 * vec3_dot(ray.dir, inter.dist);
    inter.c = vec3_dot(inter.dist, inter.dist) - (obj_p.radius * obj_p.radius);
    if ((inter.discriminant = inter.b * inter.b - 4 * inter.a * inter.c) <= 0)
        return (0);
    inter.t0 = (-inter.b - sqrtf(inter.discriminant)) / (2.0 * inter.a);
    inter.t1 = (-inter.b + sqrtf(inter.discriminant)) / (2.0 * inter.a);
    if (inter.t0 > inter.t1 && inter.t1 > 0)
        inter.t0 = inter.t1;
    if (inter.t0 < *t)
    {
        *t = inter.t0;
        return (1);
    }
    return (0);
}


/*double  inter_sphere(t_vec3 ray_ori, t_vec3 ray_dir, t_vec3 pos, float radius)
{
	double disc;
    double  a;
    double  b;
    double  c;
    t_vec3  dist;
    
    double  t0;
    double  t1;

	dist = vec3_sub(ray_ori, pos);
    display_vec3("|dist|", dist);
	a = vec3_dot(ray_dir, ray_dir);
	b = 2.0f * vec3_dot(ray_dir, dist);
	c = vec3_dot(dist, dist) - powf(radius, 2);
	if ((disc = b * b - 4 * a * c) < 0)
    {
        printf("nopito\n");
		return (-1);
    }
    t0 = (-b + sqrtf(disc)) / (2 * a);
	t1 = (-b - sqrtf(disc)) / (2 * a);
	if (t0 > t1 && t1 > 0)
	    t0 = t1;
    printf("yaya %f\n", t0);
	return (t0);
}
*/
