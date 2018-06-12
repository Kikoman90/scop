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

t_quaternion    quatInit(float theta, t_vec3 v)
{
    t_quaternion    q;

	theta = theta / 360 * (float)M_PI * 2;
	q.w = cos(theta/2);
    q.v = vecScale(n, sin(theta/2));
    return (q);
}

t_quaternion    quatInv(t_quaternion q)
{
    t_quaternion    res;
    
    res.w = q.w;
    res.v = vecInv(q.v);
    return (res);
}

t_quaternion    quatMult(t_quaternion a, t_quaternion b)
{
    t_quaternion    res;

    res.w = a.w * b.w + vecDot(a.v, b.v);
    res.v = vecScale(a.v, b.w) + vecScale(b.v, a.w) + vecCross(a.v, b.v);
    return (res);
}

t_vec3          vectorRot(t_quaternion q, t_vec3 v)
{
    t_vec3  vcv;
    t_vec3  res;

    vcv = vecCross(q.v, v);
    res = vecAdd(vecAdd(v, vecScale(vcv, 2 * q.w)), vecScale(vecCross(q.v, vcv), 2));
    return (res);
}