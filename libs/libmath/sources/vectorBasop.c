/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorBasop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 18:05:19 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/12 18:05:22 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3      vecInit(float x, float y, float z)
{
    t_vec3  v;

    v.x = x;
    v.y = y;
    v.z = z;
    return (v);
}

t_vec3      vecScale(t_vec3 v, float s)
{
    t_vec3  res;

    res.x = v.x * s;
    res.y = v.y * s;
    res.z = v.z * s;
    return (res);
}

t_vec3      vecAdd(t_vec3 a, t_vec3 b)
{
    t_vec3  res;

    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;
    return (res);
}

t_vec3      vecSub(t_vec3 a, t_vec3 b)
{
    t_vec3  res;

    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;
    return (res);
}

float       vecLength(t_vec3 v)
{
    return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}
