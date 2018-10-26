/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:52:27 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/26 15:17:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec2	vec2_f(float f)
{
	return ((t_vec2){f, f});
}

t_vec2	vec2_xy(float x, float y)
{
	return ((t_vec2){x, y});
}

t_vec2	vec2_v3(t_vec3 v)
{
	return ((t_vec2){v.x, v.y});
}

t_vec3	vec3_f(float f)
{
	return ((t_vec3){f, f, f});
}

t_vec3	vec3_xyz(float x, float y, float z)
{
	return ((t_vec3){x, y, z});
}
