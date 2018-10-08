/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:52:27 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/08 16:33:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3  vec3_f(float f)
{
    t_vec3  v;

    v.x = f;
    v.y = f;
    v.z = f;
    return (v);
}

t_vec4  vec4_f(float f)
{
    t_vec4  v;

    v.x = f;
    v.y = f;
    v.z = f;
    v.w = f;
    return (v);
}

t_vec3  vec3_xyz(float x, float y, float z)
{
    t_vec3  v;

    v.x = x;
    v.y = y;
    v.z = z;
    return (v);
}

t_vec4  vec4_xyzw(float x, float y, float z, float w)
{
    t_vec4  v;

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return (v);
}

t_vec4  vec4_v3w(t_vec3 v3, float w)
{
    t_vec4  v;

    v.x = v3.x;
    v.y = v3.y;
    v.z = v3.z;
    v.w = w;
    return (v);
}