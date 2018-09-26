/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:52:27 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/26 14:52:31 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3  vec_init_f(float f)
{
    t_vec3  v;

    v.x = f;
    v.y = f;
    v.z = f;
    return (v);
}

t_vec3      vec_init_xyz(float x, float y, float z)
{
    t_vec3  v;

    v.x = x;
    v.y = y;
    v.z = z;
    return (v);
}
