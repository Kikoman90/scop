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
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return (v);
}

t_vec3      vecAdd(t_vec3 a, t_vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return (a);
}

t_vec3      vecSub(t_vec3 a, t_vec3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return (a);
}

float       vecLength(t_vec3 v)
{
    return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}
