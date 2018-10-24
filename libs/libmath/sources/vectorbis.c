/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorbis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 16:48:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/23 16:52:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3	vec3_v4(t_vec4 v4)
{
	return ((t_vec3){{v4.x, v4.y, v4.z}});
}

t_vec4	vec4_f(float f)
{
	return ((t_vec4){{f, f, f, f}});
}

t_vec4	vec4_xyzw(float x, float y, float z, float w)
{
	return ((t_vec4){{x, y, z, w}});
}

t_vec4	vec4_v3w(t_vec3 v3, float w)
{
	return ((t_vec4){{v3.x, v3.y, v3.z, w}});
}
