/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorAdvop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 18:05:28 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/12 18:05:30 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3      vecCross(t_vec3 a, t_vec3 b)
{
    t_vec3 res;

	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
    return (res);
}

t_vec3      vecNorm(t_vec3 v)
{
    t_vec3  res;
    double	n;

	n = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    res.x = v.x / n;
    res.y = v.y / n;
    res.z = v.z / n;
	return (res);
}

float       vecDot(t_vec3 a, t_vec3 b)
{
    float	res;

	res = a.x * b.x + a.y * b.y + a.z * b.z;
	return (res);
}