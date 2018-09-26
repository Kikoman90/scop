/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_basop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:53:05 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/26 14:53:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3      vec_scale(t_vec3 v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return (v);
}

t_vec3      vec_add(t_vec3 a, t_vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return (a);
}

t_vec3      vec_sub(t_vec3 a, t_vec3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return (a);
}

float       vec_length(t_vec3 v)
{
    return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}
