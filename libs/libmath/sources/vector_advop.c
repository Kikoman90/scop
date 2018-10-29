/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_advop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:52:58 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 19:36:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3		vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	res;

	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	return (res);
}

t_vec3		vec3_norm(t_vec3 v)
{
	float	n;

	n = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (n != 0)
	{
		v.x /= n;
		v.y /= n;
		v.z /= n;
	}
	return (v);
}

float		vec3_dot(t_vec3 a, t_vec3 b)
{
	float	res;

	res = a.x * b.x + a.y * b.y + a.z * b.z;
	return (res);
}
