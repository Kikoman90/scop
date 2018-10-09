/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 17:29:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/09 17:17:41 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_quaternion    quat(void)
{
    t_quaternion    q;

    q.w = 1;
    q.v = vec3_f(0);
    //return (q);
    return (quat_norm(q));
}

t_quaternion    quat_tv(float theta, t_vec3 v) // flag parameter to normalize?
{
    t_quaternion    q;

	theta = theta * M_PI / 180.0f;
	q.w = cos(theta/2);
    q.v = vec3_scale(v, sin(theta/2));
    return (q);
    //return (quat_norm(q));
}

t_quaternion    quat_norm(t_quaternion q)
{
    float mag;

    mag = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    q.w /= mag;
    q.x /= mag;
    q.y /= mag;
    q.z /= mag;
    return (q);
}

/*
** quaternion multiplication
** (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2);
** (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2);
** (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2);
** (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2);
*/
t_quaternion    quat_mult(t_quaternion a, t_quaternion b)
{
    t_quaternion    res;

    res.w = a.w * b.w - vec3_dot(a.v, b.v);
    res.v = vec3_add(vec3_add(vec3_scale(a.v, b.w), vec3_scale(b.v, a.w)), \
                        vec3_cross(a.v, b.v));
    return (res);
}

t_vec3          vector_rot(t_quaternion q, t_vec3 v)
{
    t_vec3  vcv;
    t_vec3  res;

    vcv = vec3_cross(q.v, v);
    res = vec3_add(vec3_add(v, vec3_scale(vcv, 2 * q.w)), vec3_scale(vec3_cross(q.v, vcv), 2));
    return (res);
}


/*t_quaternion    quat_inv(t_quaternion q)
{
    t_quaternion    res;
    
    res.w = q.w;
    res.v = vec_inv(q.v);// WHERE IS VEC_INV (vec3_inv vec4_inv)
    return (res);
}*/
