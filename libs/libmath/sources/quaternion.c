/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 17:29:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/30 21:50:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_quaternion	quat(void)
{
	t_quaternion	q;

	q.w = 1;
	q.v = vec3_f(0);
	return (q);
}

t_quaternion	quat_tv(float theta, t_vec3 v)
{
	t_quaternion	q;

	theta = theta * M_PI / 180.0f;
	q.w = cos(theta / 2);
	v = vec3_norm(v);
	q.v = vec3_scale(v, sin(theta / 2));
	return (q);
}

t_quaternion	quat_norm(t_quaternion q)
{
	float mag;

	mag = sqrtf(q.w * q.w + q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z);
	q.w /= mag;
	q.v.x /= mag;
	q.v.y /= mag;
	q.v.z /= mag;
	return (q);
}

t_quaternion	quat_mult(t_quaternion a, t_quaternion b)
{
	t_quaternion	res;

	res.w = a.w * b.w - vec3_dot(a.v, b.v);
	res.v = vec3_add(\
				vec3_add(vec3_scale(a.v, b.w), vec3_scale(b.v, a.w)), \
				vec3_cross(a.v, b.v));
	return (res);
}

t_vec3			vector_rot(t_quaternion q, t_vec3 v)
{
	t_vec3	vcv;
	t_vec3	res;

	vcv = vec3_cross(q.v, v);
	res = vec3_add(vec3_add(v, vec3_scale(vcv, 2 * q.w)), \
		vec3_scale(vec3_cross(q.v, vcv), 2));
	return (res);
}
