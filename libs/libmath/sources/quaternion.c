/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 17:29:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/12 17:29:20 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

/*t_quaternion    quat_init(float theta, t_vec3 v)
{
    t_quaternion    q;

	theta = theta / 360 * (float)M_PI * 2;
	q.w = cos(theta/2);
    q.v = vec_scale(n, sin(theta/2));
    return (q);
}

t_quaternion    quat_inv(t_quaternion q)
{
    t_quaternion    res;
    
    res.w = q.w;
    res.v = vec_inv(q.v);
    return (res);
}

t_quaternion    quat_mult(t_quaternion a, t_quaternion b)
{
    t_quaternion    res;

    res.w = a.w * b.w + vec4_dot(a.v, b.v);
    res.v = vec4_scale(a.v, b.w) + vec4_scale(b.v, a.w) + vec4_cross(a.v, b.v);
    return (res);
}

t_vec3          vector_rot(t_quaternion q, t_vec3 v)
{
    t_vec3  vcv;
    t_vec3  res;

    vcv = vec4_cross(q.v, v);
    res = vec_add(vec_add(v, vec_scale(vcv, 2 * q.w)), vec_scale(vec_cross(q.v, vcv), 2));
    return (res);
}*/